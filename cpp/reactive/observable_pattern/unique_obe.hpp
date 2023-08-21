#pragma once

#include <mutex>
#include <memory>
#include <vector>
#include <functional>

#include "basic_obe.hpp"

template<typename T>
class unique_obe
{
public:
    template<typename TT>
    using functor_type = std::function<void(TT)>;
    using observable_type = basic_obe<T, functor_type>;
    using value_type = typename observable_type::value_type;
    using mutex_type = typename observable_type::mutex_type;

public:
    unique_obe() noexcept;
    unique_obe(const unique_obe & other) = delete;
    unique_obe(unique_obe && other) noexcept;
    explicit unique_obe(T value) noexcept;

    unique_obe & operator  =(const unique_obe & other) = delete;
    unique_obe & operator  =(unique_obe && other) noexcept;
    unique_obe & operator  =(value_type && value);

    value_type & get() noexcept;
    const value_type & get() const noexcept;

    void observe(functor_type<T> callback) const noexcept;

    void notify() const;

    template<typename TT>
    friend void swap(unique_obe<TT> & lhs, unique_obe<TT> & rhs);

private:
    std::unique_ptr<observable_type> m_up;
    mutable std::vector<typename observable_type::guard_type> m_observers;
};

template<typename T>
unique_obe<T>::unique_obe() noexcept :
    m_up(new observable_type),
    m_observers()
{
}

template<typename T>
unique_obe<T>::unique_obe(unique_obe && other) noexcept :
    unique_obe()
{
    swap(*this, other);
}

template<typename T>
unique_obe<T>::unique_obe(T value) noexcept :
    m_up(new observable_type(std::move(value))),
    m_observers()
{
}

template<typename T>
unique_obe<T> &
unique_obe<T>::operator  =(unique_obe && other) noexcept
{
    auto empty = unique_obe();
    swap(*this, empty);
    swap(*this, other);
    return (*this);
}

template<typename T>
unique_obe<T> &
unique_obe<T>::operator  =(value_type && value)
{
    (*m_up) = std::move(value);
    return (*this);
}

template<typename T>
typename unique_obe<T>::value_type &
unique_obe<T>::get() noexcept
{
    return m_up->get();
}

template<typename T>
typename const unique_obe<T>::value_type &
unique_obe<T>::get() const noexcept
{
    return m_up->get();
}

template<typename T>
void 
unique_obe<T>::observe(functor_type<T> callback) const noexcept
{
    m_observers.push_back(m_up->observe(std::move(callback)));
}

template<typename T>
void 
unique_obe<T>::notify() const
{
    m_up->notify();
}

template<typename T>
void swap(unique_obe<T> & lhs, unique_obe<T> & rhs)
{
    using std::swap;
    swap(lhs.m_up, rhs.m_up);

    lhs.m_observers.clear();
    rhs.m_observers.clear();
}
