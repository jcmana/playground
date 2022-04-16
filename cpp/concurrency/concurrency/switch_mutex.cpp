#include "switch_mutex.h"

void switch_mutex::lock()
{
	std::unique_lock lock(m_mutex);
	while (can_lock() == false)
	{
		m_cv.wait(lock);
	}

	m_unique_counter++;
	m_shared_counter++;

#ifdef SWITCH_MUTEX_MONITOR_DEADLOCK
	m_unique_owner = std::this_thread::get_id();
#endif
}

bool switch_mutex::try_lock()
{
	std::unique_lock lock(m_mutex);
	if (can_lock() == false)
	{
		return false;
	}

	m_unique_counter++;
	m_shared_counter++;

#ifdef SWITCH_MUTEX_MONITOR_DEADLOCK
	m_unique_owner = std::this_thread::get_id();
#endif

	return true;
}

void switch_mutex::unlock()
{
	// Critical section:
	{
		// SwitchMutex is uniquelly owned, therefore we don't need to lock
		// underlaying mutex to modify counters

		m_unique_counter--;
		m_shared_counter--;

#ifdef SWITCH_MUTEX_MONITOR_DEADLOCK
		m_unique_owner = {};
#endif
	}

	m_cv.notify_one();
}

void switch_mutex::lock_shared()
{
	std::unique_lock lock(m_mutex);
	while (can_lock_shared() == false)
	{
		m_cv.wait(lock);
	}

	m_shared_counter++;
}

bool switch_mutex::try_lock_shared()
{
	std::unique_lock lock(m_mutex);
	if (can_lock_shared() == false)
	{
		return false;
	}

	m_shared_counter++;

	return true;
}

void switch_mutex::unlock_shared()
{
	// Critical section:
	{
		// SwitchMutex is not owned uniquelly and we have to handle concurrent
		// modification of shared counter

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

#ifdef SWITCH_MUTEX_MONITOR_DEADLOCK
	m_unique_owner = std::this_thread::get_id();
#endif
}

bool switch_mutex::try_lock_unique()
{
	std::unique_lock lock(m_mutex);
	if (can_lock_unique() == false)
	{
		return false;
	}

	m_unique_counter++;

#ifdef SWITCH_MUTEX_MONITOR_DEADLOCK
	m_unique_owner = std::this_thread::get_id();
#endif

	return true;
}

void switch_mutex::unlock_unique()
{
	{
		// SwitchMutex is uniquelly owned, therefore we don't need to lock
		// underlaying mutex to modify counters

		m_unique_counter--;

#ifdef SWITCH_MUTEX_MONITOR_DEADLOCK
		m_unique_owner = {};
#endif
	}

	m_cv.notify_one();
}

bool switch_mutex::can_lock() const
{
#ifdef SWITCH_MUTEX_MONITOR_DEADLOCK
	terminate_on_deadlock();
#endif

	// Unique lock requires both shared and unique counters at zero
	return (m_shared_counter == 0 && m_unique_counter == 0);
}

bool switch_mutex::can_lock_unique() const
{
#ifdef SWITCH_MUTEX_MONITOR_DEADLOCK
	terminate_on_deadlock();
#endif

	// Upgrading from shared to unique lock both counters at zero but
	// expects shared lock is already owned
	return (m_shared_counter == 1 && m_unique_counter == 0);
}

bool switch_mutex::can_lock_shared() const
{
	// Shared lock can be concurrent with other shared locks but not
	// with unique lock, obviously
	return (m_unique_counter == 0);	
}

#ifdef SWITCH_MUTEX_MONITOR_DEADLOCK
void switch_mutex::terminate_on_deadlock() const
{
	if (m_unique_owner == std::this_thread::get_id())
	{
		// This thread is already unique owner of the mutex
		std::terminate();
	}
}
#endif
