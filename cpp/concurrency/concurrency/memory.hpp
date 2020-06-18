#pragma once

#include <mutex>
#include <utility>

#include "memory_guard.hpp"

template<typename T>
class memory
{
public:
    template<typename T>
    friend class memory_guard;

public:
    // JMTODO: enable if T is default constructible
    memory()
    {
    }

    // JMTODO: enable if T is copy constructible
    explicit memory(const T & value) :
        m_value(value)
    {
    }

    // JMTODO: enable if T is move constructible
    explicit memory(T && value) :
        m_value(std::move(value))
    {
    }

    memory(memory && other) noexcept :
        memory()
    {
        using std::swap;
        swap(*this, other);
    }

    ~memory()
    {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);

        // We now have explicit unique access to guarded memory regarding others threads.
        
        // This thread should be implicitly locked by the limitations of both memory and 
        // memory_guard API, which hopefully makes it impossible to achieve situations where
        // lock passes and somebody still could access the value.
    }

    memory & operator  =(memory && other) noexcept
    {
        using std::swap;
        swap(*this, memory());
        swap(*this, other);

        return {*this};
    }

    memory_guard<T> get()
    {
        return {*this};
    }

    friend void swap(memory & lhs, memory & rhs)
    {
        std::unique_lock<std::recursive_mutex> rhs_lock(rhs.m_mutex);
        std::unique_lock<std::recursive_mutex> lhs_lock(lhs.m_mutex);

        using std::swap;
        swap(lhs.m_value, rhs.m_value);
    }

private:
    mutable std::recursive_mutex m_mutex;
    T m_value;
};
