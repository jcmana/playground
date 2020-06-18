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
    memory()
    {
    }

    memory(const T & value) :
        m_value(value)
    {
    }

    memory(T && value) :
        m_value(std::move(value))
    {
    }

    ~memory()
    {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
    }

    memory_guard<T> get()
    {
        return {*this};
    }

private:
    mutable std::recursive_mutex m_mutex;
    T m_value;
};
