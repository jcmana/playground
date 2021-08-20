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

    template<typename ... FA>
    friend class unique_txn;

    template<typename ... FA>
    friend class shared_txn;

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

    operator std::tuple<A ...>() const
    {
        return (*m_sp);
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

template<typename ... A>
class unique_txn
{
public:
    using scoped_observable_type = shared_observable<A ...>;
    using observable_type = typename scoped_observable_type::observable_type;
    using value_type = typename scoped_observable_type::value_type;

public:
    unique_txn() = default;

    explicit unique_txn(const scoped_observable_type & observable_ref) :
        m_lock(*observable_ref.m_sp),
        m_sp(observable_ref.m_sp)
    {
    }

    ~unique_txn()
    {
        if (m_sp)
        {
            m_sp->notify();
        }
    }

    unique_txn & operator  =(const value_type & value) noexcept
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

    void release()
    {
        m_sp.reset();
    }

private:
    mutable std::unique_lock<observable_type> m_lock;
    std::shared_ptr<observable_type> m_sp;
};

template<typename ... A>
class shared_txn
{
public:
    using scoped_observable_type = shared_observable<A ...>;
    using observable_type = typename scoped_observable_type::observable_type;
    using value_type = typename scoped_observable_type::value_type;

public:
    shared_txn() :
        m_lock(),
        m_sp()
    {
    }

    shared_txn(scoped_observable_type & observable_ref) :
        m_lock(*observable_ref.m_sp),
        m_sp(observable_ref.m_sp)
    {
    }

    operator std::tuple<A ...>() const
    {
        return (*m_sp);
    }

    template<std::size_t I>
    const auto & get() const noexcept
    {
        return m_sp->get<I>();
    }

private:
    mutable std::shared_lock<observable_type> m_lock;
    std::shared_ptr<observable_type> m_sp;
};

template <typename ... A> 
struct std::tuple_size<shared_txn<A ...>> : std::tuple_size<std::tuple<A ...>>
{
};

template<std::size_t I, typename ... A> 
struct std::tuple_element<I, shared_txn<A ...>> : std::tuple_element<I, std::tuple<A ...>>
{
};

/// \brief      Waits for modfication of `observable` to meet the `predicate`.
/// \param      predicate       Functor with signature `bool(const T & value)`; should return `true`
///                             if condition is met, `false` otherwise.
template<typename F, typename ... A>
void await_if(shared_observable<A ...> o, F && predicate)
{
    std::mutex mutex;
    std::condition_variable cv;
    bool awaited = false;

    auto observer = [&predicate, &mutex, &cv, &awaited](A ... args)
    {
        if (predicate(args ...))
        {
            // Critical section:
            {
                std::unique_lock lock(mutex);
                awaited = true;
            }

            cv.notify_one();
        }
    };

    o.observe(observer);

    // Critical section:
    {
        std::unique_lock lock(mutex);
        while (awaited == false)
        {
            cv.wait(lock);
        }
    }
}

/*
/// \brief      Waits for any modfication of `observable`.
template<typename T>
void await_any(const observable<T> & o)
{
    auto predicate = [](const T &)
    {
        return true;
    };

    await_if(o, std::move(predicate));
}

/// \brief      Waits for modfication of `observable` to `value`.
template<typename T>
void await(const observable<T> & o, const T & awaited_value)
{
    auto predicate = [&](const T & value)
    {
        return value == awaited_value;
    };

    await_if(o, std::move(predicate));
}

/// \brief      Waits for modfication to one of `awaited_list` values of `observable`.
template<typename T>
void await(observable<T> & o, std::initializer_list<T> awaited_list)
{
    auto predicate = [&](const T & value)
    {
        for (const auto & awaited_value : awaited_list)
        {
            if (value == awaited_value)
            {
                return true;
            }
        }

        return false;
    };

    await_if(o, std::move(predicate));
}
*/

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

        unique_txn{composite} = std::tuple_cat(args_a, args_b);
    };

    auto observer_b = [composite, a](auto ... args) mutable
    {
        std::tuple<Ta> args_a = a;
        std::tuple<Tb> args_b = {args ...};

        unique_txn{composite} = std::tuple_cat(args_a, args_b);
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));

    return composite;
}