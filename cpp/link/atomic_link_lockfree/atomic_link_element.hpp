#pragma once

#include <atomic>

/*
struct atomic_link_element
{

    void lock_internal()
    {
        while (m_element_ptr.compare_exchange_strong(m_element_ptr_static, PTR_LOCKED, std::memory_order_acquire)) 
        {
            std::this_thread::yield();
        }
    }

    void unlock_internal()
    {
        m_element_ptr.store(m_element_ptr_static, std::memory_order_release);
    }

    void lock()
    {
        lock_internal();
        if (m_element_ptr != PTR_UNLINKED)
        {
            //->lock_internal();
        }
    }

    void unlock()
    {
        unlock_internal();
    }

    // I GUESS THIS WON'T WORK
    // practically same as using mutexes, only with spinlocks, because locking needs
    // to be part of public interface

    atomic_link_element * m_element_ptr;
    std::atomic<std::uint8_t> m_state;
};
*/