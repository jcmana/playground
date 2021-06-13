#pragma once

#include <functional>
#include <tuple>

#include "../../callback_store/atomic_callback_store/atomic_callback_guard.hpp"
#include "observable.hpp"
#include "observable_utility.hpp"

template<typename T, typename A, typename B>
struct observable_composite;

template<typename T, typename A, typename B>
struct observable_composite : observable<T>
{
    using functor_type = std::function<T(const A &, const B &)>;

    observable_composite(functor_type functor, const observable<A> & a, const observable<B> & b) :
        m_functor(std::move(functor))
    {
        observable<T>::set(m_functor(a.get(), b.get()));

        auto callback = [&](const A & value_a, const B & value_b)
        {
            observable<T>::set(m_functor({value_a}, {value_b}));
        };

        std::tie(m_guard_a, m_guard_b) = join(callback, a, b);
    }

    functor_type m_functor;

    typename observable<A>::guard_type m_guard_a;
    typename observable<B>::guard_type m_guard_b;
};

template<typename A, typename B>
struct observable_composite<std::tuple<A, B>, A, B>
{
};