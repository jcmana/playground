#pragma once

#include <thread>
#include <atomic>

class spinlock
{
public:
    spinlock() :
        m_locked()
    {
    }

    void lock() 
    {
        while (true)
        {
            // exchange stricly m_locked false with true (required unlocked before locking)
            if (m_locked.exchange(true, std::memory_order_acquire) == false)
            //bool expected = false;
            //if (m_locked.compare_exchange_weak(expected, true, std::memory_order_acquire))
            //if (m_locked.test_and_set(std::memory_order_acquire) == false)
            {
                break;
            }

            while (m_locked.load(std::memory_order_relaxed))
            {
                std::this_thread::yield();
            }
        }
    }

    void unlock()
    {
        m_locked.store(false, std::memory_order_release);
        //m_locked.clear();

    }

private:
    std::atomic<bool> m_locked;
    //std::atomic_flag m_locked;
};
