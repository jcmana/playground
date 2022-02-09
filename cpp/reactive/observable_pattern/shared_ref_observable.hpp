#pragma once

#include <memory>
#include <vector>

#include "basic_observable.hpp"

template<typename ... A>
class shared_ref_observable;

template<typename ... A>
class shared_ref_observable<std::unique_ptr<A> ...>
{
public:
    template<typename ... FA>
    using functor_type = std::function<void(const FA & ...)>;
    using observable_type = basic_observable<functor_type, std::unique_ptr<A> ...>;
    using guard_type = typename observable_type::guard_type;
    using value_type = typename observable_type::value_type;
    using mutex_type = typename observable_type::mutex_type;

    shared_ref_observable() noexcept :
        m_sp(new observable_type()),
        m_observers()
    {
    }

    shared_ref_observable(std::unique_ptr<A> ... args) noexcept :
        m_sp(new observable_type(std::forward<std::unique_ptr<A>>(args) ...)),
        m_observers()
    {
    }

    void observe(functor_type<A ...> callback) noexcept
    {
        auto callback_wrapper = [callback](const std::unique_ptr<A> & ... args)
        {
            callback(*args ...);
        };
        m_observers.push_back(m_sp->observe(std::move(callback_wrapper)));
    }

public:
    std::shared_ptr<observable_type> m_sp;
    std::vector<typename observable_type::guard_type> m_observers;
};