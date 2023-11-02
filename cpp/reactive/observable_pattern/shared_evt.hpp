#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "basic_evt.hpp"

/// \brief      Shared reference to an event.
template<typename T>
class shared_evt
{
public:
    template<typename FT>
    using functor_type = std::function<void(FT)>;
    using functor_void_type = std::function<void()>;

    using observable_type = typename basic_evt<std::remove_cv_t<T>, functor_type>;
    using guard_type = typename observable_type::guard_type;

public:
    shared_evt() :
        m_sp(new observable_type()),
        m_observers()
    {
    }

    shared_evt(const shared_evt & other) :
        m_sp(other.m_sp),
        m_observers()
    {
    }

    shared_evt(shared_evt && other) :
        shared_obe()
    {
        swap(*this, other);
    }

    void observe(functor_type<const T &> callback)
    {
        m_observers.push_back(observe_scoped(std::move(callback)));
    }

    auto observe_scoped(functor_type<const T &> callback)
    {
        return m_sp->observe(std::move(callback));
    }

    template<typename C>
    void observe(void(C:: * f)(const T &), C * ptr)
    {
        m_observers.push_back(observe_scoped(f, ptr));
    }

    template<typename C>
    auto observe_scoped(void(C:: * f)(const T &), C * ptr)
    {
        return m_sp->observe(std::bind(f, ptr, std::placeholders::_1));
    }

    void notify(const T & value) const
    {
        m_sp->notify(value);
    }

    template<typename TT>
    friend void swap(shared_evt<TT> & lhs, shared_evt<TT> & rhs) noexcept
    {
        using std::swap;
        swap(lhs.m_sp, rhs.m_sp);

        lhs.m_observers.clear();
        rhs.m_observers.clear();
    }

private:
    std::shared_ptr<observable_type> m_sp;
    std::vector<guard_type> m_observers;
};