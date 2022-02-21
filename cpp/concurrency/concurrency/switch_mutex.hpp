#pragma once

#include <optional>

#include "switch_mutex.h"

template<typename T>
class shared_lock;

template<typename T>
class unique_lock
{
public:
	template<typename FT>
	friend class shared_lock;

public:
	unique_lock() :
		m_mutex_ptr(nullptr)
	{
	}

	explicit unique_lock(T & mutex) :
		m_mutex_ptr(&mutex)
	{
		m_mutex_ptr->lock();
	}

	unique_lock(unique_lock && lock) :
		m_mutex_ptr(lock.m_mutex_ptr)
	{
		lock.m_mutex_ptr = nullptr;
	}

	unique_lock(shared_lock<T> && lock) :
		m_mutex_ptr(lock.m_mutex_ptr)
	{
		lock.m_mutex_ptr = nullptr;

		if (m_mutex_ptr)
		{
			m_mutex_ptr->lock_unique();
		}
	}

	~unique_lock()
	{
		if (m_mutex_ptr)
		{
			m_mutex_ptr->unlock();
		}
	}

	unique_lock & operator  =(unique_lock && lock)
	{
		m_mutex_ptr = lock.m_mutex_ptr;
		lock.m_mutex_ptr = nullptr;

		return (*this);
	}

	unique_lock & operator  =(shared_lock<T> && lock)
	{
		m_mutex_ptr = lock.m_mutex_ptr;
		lock.m_mutex_ptr = nullptr;

		if (m_mutex_ptr)
		{
			m_mutex_ptr->lock_unique();
		}

		return (*this);
	}

private:
	T * m_mutex_ptr;
};

template<typename T>
class shared_lock
{
public:
	template<typename FT>
	friend class unique_lock;

public:
	shared_lock() :
		m_mutex_ptr(nullptr)
	{
	}

	explicit shared_lock(T & mutex) :
		m_mutex_ptr(&mutex)
	{
		m_mutex_ptr->lock_shared();
	}

	shared_lock(shared_lock && lock) :
		m_mutex_ptr(lock.m_mutex_ptr)
	{
		lock.m_mutex_ptr = nullptr;
	}

	shared_lock(unique_lock<T> && lock) :
		m_mutex_ptr(lock.m_mutex_ptr)
	{
		lock.m_mutex_ptr = nullptr;

		if (m_mutex_ptr)
		{
			m_mutex_ptr->unlock_unique();
		}
	}

	~shared_lock()
	{
		if (m_mutex_ptr)
		{
			m_mutex_ptr->unlock_shared();
		}
	}

	shared_lock & operator  =(shared_lock && lock)
	{
		m_mutex_ptr = lock.m_mutex_ptr;
		lock.m_mutex_ptr = nullptr;

		return (*this);
	}

	shared_lock & operator  =(unique_lock<T> && lock)
	{
		m_mutex_ptr = lock.m_mutex_ptr;
		lock.m_mutex_ptr = nullptr;

		if (m_mutex_ptr)
		{
			m_mutex_ptr->unlock_unique();
		}

		return (*this);
	}

private:
	T * m_mutex_ptr;
};

/// \brief		Holds either unique locked or shared locked `switch_mutex`.
template<typename T>
class switch_lock
{
public:
	template<typename FT>
	friend class unique_lock;

	template<typename FT>
	friend class shared_lock;

public:
	switch_lock() :
		m_lock(std::nullopt)
	{
	}

	switch_lock(unique_lock<T> && lock) :
		m_lock(std::move(lock))
	{
	}

	switch_lock(shared_lock<T> && lock) :
		m_lock(std::move(lock))
	{
	}


	/// \brief		Switch from shared to unique lock.
	/// \throws		std::exception
	void lock_unique()
	{
		if (std::holds_alternative<std::nullopt_t>(m_lock))
		{
			throw std::exception("Invalid lock");
		}

		if (std::holds_alternative<unique_lock<T>>(m_lock))
		{
			throw std::exception("Unique lock already owned");
		}

		unique_lock<T> lock = std::move(std::get<shared_lock<T>>(m_lock));
		m_lock = std::move(lock);
	}

	/// \brief		Switch from unique to shared lock.
	/// \throws		std::exception
	void unlock_unique()
	{
		if (std::holds_alternative<std::nullopt_t>(m_lock))
		{
			throw std::exception("Invalid lock");
		}

		if (std::holds_alternative<shared_lock<T>>(m_lock))
		{
			throw std::exception("Shared lock already owned");
		}

		shared_lock<T> lock = std::move(std::get<unique_lock<T>>(m_lock));
		m_lock = std::move(lock);
	}

private:
	std::variant<unique_lock<T>, shared_lock<T>, std::nullopt_t> m_lock;
};
