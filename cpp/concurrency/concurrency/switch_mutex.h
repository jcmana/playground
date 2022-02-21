#pragma once

#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <variant>
#include <optional>

class switch_mutex
{
public:
	// SharedMutex implementation:
	void lock();
	void unlock();
	void lock_shared();
	void unlock_shared();

	/// \brief      Upgrades the mutex lock from shared to unique.
	///
	/// Expects mutex in `lock_shared()` state.
	void lock_unique();

	/// \brief      Downgrades the mutex lock from unique to shared.
	///
	/// Expects mutex in `lock()` state. 
	void unlock_unique();

private:
	std::mutex m_mutex;
	std::condition_variable m_cv;
	unsigned int m_unique_counter = 0;
	unsigned int m_shared_counter = 0;
};

class unique_lock
{
public:
	friend class shared_lock;

public:
	unique_lock();
	unique_lock(switch_mutex & mutex);
	unique_lock(unique_lock && lock);
	unique_lock(shared_lock && lock);

	unique_lock & operator  =(unique_lock && lock);

	~unique_lock();

private:
	switch_mutex * m_mutex_ptr;
};

class shared_lock
{
public:
	friend class unique_lock;

public:
	shared_lock();
	shared_lock(switch_mutex & mutex);
	shared_lock(shared_lock && lock);
	shared_lock(unique_lock && lock);

	shared_lock & operator  =(shared_lock && lock);

	~shared_lock();

private:
	switch_mutex * m_mutex_ptr;
};

/// \brief		Holds either unique locked or shared locked `switch_mutex`.
class switch_lock
{
public:
	friend class unique_lock;
	friend class shared_lock;

public:
	switch_lock();
	switch_lock(unique_lock && lock);
	switch_lock(shared_lock && lock);

	/// \brief		Switch from shared to unique lock.
	/// \throws		std::exception
	void lock_unique();
	/// \brief		Switch from unique to shared lock.
	/// \throws		std::exception
	void unlock_unique();

private:
	std::variant<unique_lock, shared_lock, std::nullopt_t> m_lock;
};
