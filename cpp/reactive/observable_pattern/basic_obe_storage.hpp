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
class basic_obe_storage<std::unique_ptr<T>>
{
public:
    basic_obe_storage() :
        m_up(new T())
    {
    }

    basic_obe_storage(std::unique_ptr<T> up) :
        m_up(std::move(up))
    {
    }

    T & get()
    {
        return (*m_up);
    }

    const T & get() const
    {
        return (*m_up);
    }

private:
    std::unique_ptr<T> m_up;
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
