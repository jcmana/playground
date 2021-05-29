#pragma once

#include <mutex>

namespace asymetric_link_element
{

struct master;
struct slave;

struct master
{
    ~master();

    void lock() const;
    void unlock() const;

    mutable std::mutex m_mutex;
    slave * m_element_ptr;
};

struct slave
{
    ~slave();

    void lock() const;
    void unlock() const;

    mutable std::mutex m_mutex;
    master * m_element_ptr;
};

master::~master()
{
    std::unique_lock lock_master(m_mutex);
    if (m_element_ptr)
    {
        std::unique_lock lock_slave(m_element_ptr->m_mutex);
        m_element_ptr->m_element_ptr = nullptr;
        m_element_ptr = nullptr;
    }
}

void master::lock() const
{
    m_mutex.lock();

    if (m_element_ptr)
    {
        m_element_ptr->m_mutex.lock();
    }
}

void master::unlock() const
{
    if (m_element_ptr)
    {
        m_element_ptr->m_mutex.unlock();
    }

    m_mutex.unlock();
}

slave::~slave()
{
    {
        std::unique_lock lock_slave(m_mutex);
        if (m_element_ptr)
        {
            lock_slave.unlock();

            // JMTODO: whole ~master can run here and therefore m_element_ptr might be deleted

            std::unique_lock lock_master(m_element_ptr->m_mutex);
            if (m_element_ptr)
            {
                m_element_ptr->m_element_ptr = nullptr;
                m_element_ptr = nullptr;
            }
        }
    }

    std::unique_lock lock_slave(m_mutex);
}

void slave::lock() const
{
    while (true)
    {
        m_mutex.lock();

        if (m_element_ptr)
        {
            if (m_element_ptr->m_mutex.try_lock())
            {
                break;
            }

            m_mutex.unlock();
        }

        std::this_thread::yield();
    }
}

void slave::unlock() const
{
    if (m_element_ptr)
    {
        m_element_ptr->m_mutex.unlock();
    }

    m_mutex.unlock();
}

};
