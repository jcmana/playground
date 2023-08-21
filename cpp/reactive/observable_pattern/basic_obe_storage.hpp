#pragma once

#include <memory>

/// \brief      Holds store value in `basic_obe`.
/// \param      T       Held value type.
template<typename T>
class basic_obe_storage;

template<typename T>
class basic_obe_storage
{
public:
    using value_type = T;
    using observed_type = value_type;

public:
    basic_obe_storage() :
        m_value()
    {
    }

    basic_obe_storage(value_type value) :
        m_value(std::move(value))
    {
    }

    observed_type & get()
    {
        return m_value;
    }

    const observed_type & get() const
    {
        return m_value;
    }

private:
    value_type m_value;
};

template<typename T>
class basic_obe_storage<std::unique_ptr<T>>
{
public:
    using value_type = T;
    using observed_type = std::unique_ptr<T>;

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
    using value_type = std::shared_ptr<T>;
    using observed_type = T;

public:
    basic_obe_storage() :
        m_sp(new T())
    {
    }

    basic_obe_storage(value_type sp) :
        m_sp(std::move(sp))
    {
    }

    observed_type & get()
    {
        return (*m_sp);
    }

    const observed_type & get() const
    {
        return (*m_sp);
    }

private:
    value_type m_sp;
};
