#pragma once

#include <thread>
#include <atomic>

class spinlock
{
public:
    spinlock() :
        m_locked(false)
    {
    }

    void lock() 
    {
        while (true)
        {
            // exchange stricly m_locked false with true (required unlocked before locking)
            if (m_locked.exchange(true, std::memory_order_acquire) == false)
            {
                break;
            }

            while (m_locked.load(std::memory_order_relaxed))
            {
                std::this_thread::yield();
            }
        }
    }

    bool try_lock() 
    {
        // exchange stricly m_locked false with true (required unlocked before locking)
        if (m_locked.exchange(true, std::memory_order_acquire) == false)
        {
            return true;
        }

        return false;
    }

    void unlock()
    {
        m_locked.store(false, std::memory_order_release);

    }

private:
    std::atomic<bool> m_locked;
};
