#pragma once

#include "../../concurrency/concurrency/memory.hpp"

class atomic_link_element
{
public:
    atomic_link_element() noexcept :
        m_element_ptr(nullptr)
    {
    }

    atomic_link_element(atomic_link_element * element_ptr) noexcept :
        m_element_ptr(element_ptr)
    {
        if (m_element_ptr)
        {
            std::unique_lock<std::mutex> that_lock(m_element_ptr->m_mutex);

            if (m_element_ptr->m_element_ptr == nullptr)
            {
                m_element_ptr->m_element_ptr = this;
            }
        }
    }

    ~atomic_link_element()
    {
        release();
    }

    void lock() const
    {
        m_mutex.lock();
    }

    void unlock() const
    {
        m_mutex.unlock();
    }

    bool is_linked() const noexcept
    {
        return m_element_ptr != nullptr;
    }

    void release() noexcept
    {
        std::unique_lock<std::mutex> this_lock(m_mutex);

        if (m_element_ptr)
        {
            std::unique_lock<std::mutex> that_lock(m_element_ptr->m_mutex);

            m_element_ptr->m_element_ptr = nullptr;
            m_element_ptr = nullptr;
        }
    }

private:
    mutable std::mutex m_mutex;
    atomic_link_element * m_element_ptr;
};
