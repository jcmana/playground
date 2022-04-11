#pragma once

#include <memory>
#include <vector>

#include "basic_obe.hpp"

template<typename T>
class shared_ref_observable;

template<typename T>
class shared_ref_observable<std::unique_ptr<T>>
{
public:
    template<typename TT>
    using functor_type = std::function<void(const TT &)>;
    using observable_type = basic_obe<functor_type, std::unique_ptr<T>>;
    using guard_type = typename observable_type::guard_type;
    using value_type = typename observable_type::value_type;
    using mutex_type = typename observable_type::mutex_type;

    shared_ref_observable() noexcept :
        m_sp(new observable_type(std::unique_ptr<T>(new T()))),
        m_observers()
    {
    }

    shared_ref_observable(std::unique_ptr<T> up) noexcept :
        m_sp(new observable_type(std::move(up))),
        m_observers()
    {
    }

    void observe(functor_type<T> callback) noexcept
    {
        auto callback_wrapper = [callback](const std::unique_ptr<T> & up)
        {
            callback(*up);
        };
        m_observers.push_back(m_sp->observe(std::move(callback_wrapper)));
    }

public:
    std::shared_ptr<observable_type> m_sp;
    std::vector<typename observable_type::guard_type> m_observers;
};