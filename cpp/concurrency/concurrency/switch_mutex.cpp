#include "switch_mutex.h"

void switch_mutex::lock()
{
	std::unique_lock lock(m_mutex);
	while (m_shared_counter > 0 || m_unique_counter > 0)
	{
		m_cv.wait(lock);
	}

	m_unique_counter++;
	m_shared_counter++;
}

void switch_mutex::unlock()
{
	m_unique_counter--;
	m_shared_counter--;
	m_cv.notify_one();
}

void switch_mutex::lock_shared()
{
	std::unique_lock lock(m_mutex);
	while (m_unique_counter > 0)
	{
		m_cv.wait(lock);
	}

	m_shared_counter++;
}

void switch_mutex::unlock_shared()
{
	{
		std::unique_lock lock(m_mutex);
		m_shared_counter--;
	}
	m_cv.notify_one();
}

void switch_mutex::lock_unique()
{
	std::unique_lock lock(m_mutex);
	while (m_shared_counter > 1 || m_unique_counter > 0)
	{
		m_cv.wait(lock);
	}

	m_unique_counter++;
}

void switch_mutex::unlock_unique()
{
	m_unique_counter--;
	m_cv.notify_one();
}

unique_lock::unique_lock() :
	m_mutex_ptr(nullptr)
{
}

unique_lock::unique_lock(switch_mutex & mutex) :
	m_mutex_ptr(&mutex)
{
	m_mutex_ptr->lock();
}

unique_lock::unique_lock(unique_lock && lock) :
	m_mutex_ptr(lock.m_mutex_ptr)
{
	lock.m_mutex_ptr = nullptr;
}

unique_lock::unique_lock(shared_lock && lock) :
	m_mutex_ptr(lock.m_mutex_ptr)
{
	lock.m_mutex_ptr = nullptr;

	if (m_mutex_ptr)
	{
		m_mutex_ptr->lock_unique();
	}
}

unique_lock & unique_lock::operator  =(unique_lock && lock)
{
	m_mutex_ptr = lock.m_mutex_ptr;
	lock.m_mutex_ptr = nullptr;

	return (*this);
}

unique_lock::~unique_lock()
{
	if (m_mutex_ptr)
	{
		m_mutex_ptr->unlock();
	}
}

shared_lock::shared_lock() :
	m_mutex_ptr(nullptr)
{
}

shared_lock::shared_lock(switch_mutex & mutex) :
	m_mutex_ptr(&mutex)
{
	m_mutex_ptr->lock_shared();
}

shared_lock::shared_lock(shared_lock && lock) :
	m_mutex_ptr(lock.m_mutex_ptr)
{
	lock.m_mutex_ptr = nullptr;
}

shared_lock::shared_lock(unique_lock && lock) :
	m_mutex_ptr(lock.m_mutex_ptr)
{
	lock.m_mutex_ptr = nullptr;

	if (m_mutex_ptr)
	{
		m_mutex_ptr->unlock_unique();
	}
}

shared_lock & shared_lock::operator  =(shared_lock && lock)
{
	m_mutex_ptr = lock.m_mutex_ptr;
	lock.m_mutex_ptr = nullptr;

	return (*this);
}

shared_lock::~shared_lock()
{
	if (m_mutex_ptr)
	{
		m_mutex_ptr->unlock_shared();
	}
}

switch_lock::switch_lock() :
	m_lock(std::nullopt)
{
}

switch_lock::switch_lock(unique_lock && lock) :
	m_lock(std::move(lock))
{
}

switch_lock::switch_lock(shared_lock && lock) :
	m_lock(std::move(lock))
{
}

void switch_lock::lock_unique()
{
	if (std::holds_alternative<std::nullopt_t>(m_lock))
	{
		throw std::exception("Invalid lock");
	}

	if (std::holds_alternative<unique_lock>(m_lock))
	{
		throw std::exception("Unique lock already owned");
	}

	unique_lock lock = std::move(std::get<shared_lock>(m_lock));
	m_lock = std::move(lock);
}

void switch_lock::unlock_unique()
{
	if (std::holds_alternative<std::nullopt_t>(m_lock))
	{
		throw std::exception("Invalid lock");
	}

	if (std::holds_alternative<shared_lock>(m_lock))
	{
		throw std::exception("Shared lock already owned");
	}

	shared_lock lock = std::move(std::get<unique_lock>(m_lock));
	m_lock = std::move(lock);
}
