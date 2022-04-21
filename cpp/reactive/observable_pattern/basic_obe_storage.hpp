#pragma once

#include <memory>

template<typename T>
class basic_obe_storage;

template<typename T>
class basic_obe_storage
{
public:
    basic_obe_storage() :
        m_value()
    {
    }

    basic_obe_storage(T value) :
        m_value(std::move(value))
    {
    }

    T & get()
    {
        return m_value;
    }

    const T & get() const
    {
        return m_value;
    }

private:
    T m_value;
};

template<typename T>
class basic_obe_storage<std::shared_ptr<T>>
{
public:
    basic_obe_storage() :
        m_sp(new T())
    {
    }

    basic_obe_storage(std::shared_ptr<T> sp) :
        m_sp(std::move(sp))
    {
    }

    T & get()
    {
        return (*m_sp);
    }

    const T & get() const
    {
        return (*m_sp);
    }

private:
    std::shared_ptr<T> m_sp;
};
