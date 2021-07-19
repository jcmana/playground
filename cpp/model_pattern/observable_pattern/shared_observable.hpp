#pragma once

#include <vector>

#include "observable.hpp"

template<typename ... A>
class shared_observable;

template<typename T>
class shared_observable<T>
{
public:
    using observable_type = observable<T>;
    using value_type = typename observable_type::value_type;
    using guard_type = typename observable_type::guard_type;

    shared_observable() :
        m_sp(new observable_type)
    {
    }

    shared_observable(const shared_observable & other) :
        m_sp(other.m_sp)
    {
    }

    void set(const value_type & value)
    {
        m_sp->set(std::move(value));
    }

    const value_type & get() const
    {
        return m_sp->get();
    }

    template<typename F>
    void observe(F && functor)
    {
        auto guard = m_sp->observe(std::move(functor));
        m_guards.push_back(std::move(guard));
    }

    const std::shared_ptr<observable_type> m_sp;
    std::vector<guard_type> m_guards;
};

template<typename ... A>
class shared_observable
{
public:
    using observable_type = observable<A ...>;
    using value_type = typename observable_type::value_type;
    using guard_type = typename observable_type::guard_type;

    shared_observable() :
        m_sp(new observable_type)
    {
    }

    shared_observable(const shared_observable & other) :
        m_sp(other.m_sp)
    {
    }

    void set(const value_type & value)
    {
        m_sp->set(std::move(value));
    }

    void set(const A & ... args)
    {
        m_sp->set(args ...);
    }

    const value_type & get() const
    {
        return m_sp->get();
    }

    template<typename F>
    void observe(F && functor)
    {
        auto guard = m_sp->observe(std::move(functor));
        m_guards.push_back(std::move(guard));

        // JMTODO: scan for unused guards instead of pushing back
    }

    const std::shared_ptr<observable_type> m_sp;
    std::vector<guard_type> m_guards;
};

template<typename F, typename Ta, typename Tb>
void join(F && functor, shared_observable<Ta> & a, shared_observable<Tb> & b)
{
    auto observer_a = [functor, b](const Ta & value) mutable
    {
        functor(value, b.get());
    };

    auto observer_b = [functor, a](const Tb & value) mutable
    {
        functor(a.get(), value);
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));
}

template<typename Ta, typename Tb>
auto join(shared_observable<Ta> & a, shared_observable<Tb> & b)
{
    shared_observable<Ta, Tb> composite;

    auto observer_a = [composite, b](const Ta & value) mutable
    {
        composite.set(value, b.get());
    };

    auto observer_b = [composite, a](const Tb & value) mutable
    {
        composite.set({a.get(), value});
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));

    return composite;
}