#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "basic_observable.hpp"

template<typename ... A>
class shared_observable
{
public:
    template<typename ... FA>
    using functor_type = std::function<void(FA ...)>;
    using observable_type = basic_observable<functor_type, A ...>;
    using value_type = typename observable_type::value_type;
    using mutex_type = typename observable_type::mutex_type;

public:
    shared_observable() noexcept :
        m_sp(new observable_type),
        m_observers()
    {
    }

    shared_observable(const shared_observable & other) noexcept :
        m_sp(other.m_sp),
        m_observers()
    {
    }

    shared_observable(shared_observable && other) noexcept :
        shared_observable()
    {
        swap(*this, other);
    }

    explicit shared_observable(A ... args) noexcept :
        m_sp(new observable_type(std::forward<A>(args) ...)),
        m_observers()
    {
    }

    shared_observable & operator  =(const shared_observable & other) noexcept
    {
        m_sp = other.m_sp;
        return (*this);
    }

    shared_observable & operator  =(shared_observable && other) noexcept
    {
        auto empty = shared_observable();
        swap(*this, empty);
        swap(*this, other);
        return (*this);
    }

    shared_observable & operator  =(const value_type & value) noexcept
    {
        (*m_sp) = value;
        return (*this);
    }

    operator std::tuple<A ...>() const
    {
        return (*m_sp);
    }

    template<std::size_t I>
    auto & get() noexcept
    {
        return m_sp->get<I>();
    }

    template<std::size_t I>
    const auto & get() const noexcept
    {
        return m_sp->get<I>();
    }

    void observe(functor_type<A ...> callback) const noexcept
    {
        m_observers.push_back(m_sp->observe(std::move(callback)));
    }

    void notify() const
    {
        m_sp->notify();
    }

    template<typename ... FA>
    friend void swap(shared_observable & lhs, shared_observable & rhs)
    {    
        using std::swap;
        swap(lhs.m_sp, rhs.m_sp);

        lhs.m_observers.clear();
        rhs.m_observers.clear();
    }

public:
    // SharedMutex implementation:
    void lock() const
    {
        m_sp->lock();
    }

    bool try_lock() const
    {
        return m_sp->try_lock();
    }

    void unlock()
    {
        m_sp->unlock();
    }

    void lock_shared() const
    {
        m_sp->lock_shared();
    }

    bool try_lock_shared() const
    {
        return m_sp->try_lock_shared();
    }

    void unlock_shared()
    {
        m_sp->unlock_shared();
    }

private:
    std::shared_ptr<observable_type> m_sp;
    mutable std::vector<typename observable_type::guard_type> m_observers;
};

template <typename ... A> 
struct std::tuple_size<shared_observable<A ...>> : std::tuple_size<std::tuple<A ...>>
{
};

template<std::size_t I, typename ... A> 
struct std::tuple_element<I, shared_observable<A ...>> : std::tuple_element<I, std::tuple<A ...>>
{
};

template<typename F, typename Ta, typename Tb>
void join(F && functor, shared_observable<Ta> & a, shared_observable<Tb> & b)
{
    auto observer_a = [functor, b](auto ... args) mutable
    {
        std::tuple<Ta> args_a = {args ...};
        std::tuple<Tb> args_b = b;

        std::apply(functor, std::tuple_cat(args_a, args_b));
    };

    auto observer_b = [functor, a](auto ... args) mutable
    {
        std::tuple<Ta> args_a = a;
        std::tuple<Tb> args_b = {args ...};

        std::apply(functor, std::tuple_cat(args_a, args_b));
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));
}

template<typename Ta, typename Tb>
auto join(shared_observable<Ta> & a, shared_observable<Tb> & b)
{
    shared_observable<Ta, Tb> composite;

    auto observer_a = [composite, b](auto ... args) mutable
    {
        std::tuple<Ta> args_a = {args ...};
        std::tuple<Tb> args_b = b;

        composite = std::tuple_cat(args_a, args_b);
        composite.notify();
    };

    auto observer_b = [composite, a](auto ... args) mutable
    {
        std::tuple<Ta> args_a = a;
        std::tuple<Tb> args_b = {args ...};

        composite = std::tuple_cat(args_a, args_b);
        composite.notify();
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));

    return composite;
}