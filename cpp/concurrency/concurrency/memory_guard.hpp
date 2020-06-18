#pragma once

#include <utility>

template<typename T>
class memory_guard
{
public:
    template<typename T>
    friend class memory;

public:
    memory_guard(memory<T> & memory) :
        m_memory(memory)
    {
        m_memory.m_mutex.lock();
    }

    ~memory_guard()
    {
        m_memory.m_mutex.unlock();
    }

    // JMTODO: enable if T is copy constructible
    memory_guard & operator  =(const T & value) noexcept
    {
        m_memory.m_value = std::move(value);
        return (*this);
    }

    // JMTODO: enable if T is move constructible
    memory_guard & operator  =(T && value) noexcept
    {
        m_memory.m_value = std::move(value);
        return (*this);
    }

    operator T()
    {
        return m_memory.m_value;
    }

private:
    memory<T> & m_memory;
};
