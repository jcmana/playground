#pragma once

#include <mutex>
#include <condition_variable>
#include <variant>

/// \brief		Unique or shared mutex with atomic switching between ownership.
class switch_mutex
{
public:
	// SwitchMutex concept:
	// - implements SharedMutex
	// - void lock_unique()
	// - void unlock_unique()
	// - bool try_lock_unique()

public:
	// SwitchMutex implementation:
	void lock();
	bool try_lock();
	void unlock();
	void lock_shared();
	bool try_lock_shared();
	void unlock_shared();

	/// \brief      Upgrades the mutex lock from shared to unique.
	///
	/// Expects mutex in `lock_shared()` state.
	void lock_unique();

	/// \brief		Tries to upgrade the mutex lock without blocking.
	/// \returns	`true` if upgraded succesfully; `false` otherwise.
	bool try_lock_unique();

	/// \brief      Downgrades the mutex lock from unique to shared.
	///
	/// Expects mutex in `lock()` state. 
	void unlock_unique();


private:
	bool can_lock();
	bool can_lock_unique();
	bool can_lock_shared();

private:
	std::mutex m_mutex;
	std::condition_variable m_cv;
	unsigned int m_unique_counter = 0;
	unsigned int m_shared_counter = 0;
};
