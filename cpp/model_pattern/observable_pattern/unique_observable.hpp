#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "basic_observable.hpp"

template<typename ... A>
class unique_observable
{
public:
    template<typename ... FA>
    using functor_type = std::function<void(FA ...)>;
    using observable_type = basic_observable<functor_type, A ...>;
    using value_type = typename observable_type::value_type;

public:
    unique_observable() :
        m_up(new observable_type)
    {
    }

    unique_observable(A ... args) :
        m_up(new observable_type(std::forward<A>(args) ...))
    {
    }

    unique_observable(const unique_observable & other) = delete;

    unique_observable(unique_observable && other) :
        m_up(std::move(other.m_up)),
        m_observers()
    {
        other.m_up.reset(new observable_type);
        other.m_observers.clear();
    }

    unique_observable & operator  =(value_type && value)
    {
        *m_up = std::move(value);
        return (*this);
    }

    void observe(functor_type<A ...> callback) const noexcept
    {
        m_observers.push_back(m_up->observe(std::move(callback)));
    }

    void notify() const
    {
        m_up->notify();
    }

private:
    std::unique_ptr<observable_type> m_up;
    mutable std::vector<typename observable_type::guard_type> m_observers;
};