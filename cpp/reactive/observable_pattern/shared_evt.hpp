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

    void observe(functor_type<const T &> callback)
    {
        m_observers.push_back(m_sp->observe(std::move(callback)));
    }

    auto observe_scoped(functor_type<const T &> callback)
    {
        return m_sp->observe(std::move(callback));
    }

    void notify(const T & value) const
    {
        m_sp->notify(value);
    }

private:
    std::shared_ptr<observable_type> m_sp;
    std::vector<guard_type> m_observers;
};