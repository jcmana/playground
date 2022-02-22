#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "basic_observable.hpp"

template<typename ... A>
class shared_observable;

/// \brief      Shared reference to an observable value with observers.
template<typename ... A>
class shared_observable
{
public:
    template<typename ... FA>
    using functor_type = std::function<void(FA ...)>;
    using observable_type = basic_observable<functor_type, A ...>;
    using guard_type = typename observable_type::guard_type;
    using value_type = typename observable_type::value_type;
    using mutex_type = typename observable_type::mutex_type;

    template<typename ... FA>
    friend class unique_txn;

    template<typename ... FA>
    friend class shared_txn;

public:
    /// \brief      Constructor, creates empty observable with default value.
    shared_observable() noexcept :
        m_sp(new observable_type),
        m_observers()
    {
    }

    /// \brief      Copy constructor, copies shared reference, clears observers.
    shared_observable(const shared_observable & other) noexcept :
        m_sp(other.m_sp),
        m_observers()
    {
    }

    /// \brief      Move constructor, moves shared reference, clears observers.
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

    void observe(functor_type<A ...> callback) noexcept
    {
        m_observers.push_back(m_sp->observe(std::move(callback)));
    }

    auto observe_scoped(functor_type<A ...> callback) noexcept
    {
        return m_sp->observe(std::move(callback));
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
    std::vector<guard_type> m_observers;
};

/// \brief      Shared reference to an no-value observable.
template<>
class shared_observable<void>
{
public:
    using functor_type = std::function<void()>;
    using observable_type = atomic_callback_store<functor_type>;
    using guard_type = atomic_callback_guard<functor_type>;

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

    void observe(functor_type callback) noexcept
    {
        m_observers.push_back(m_sp->subscribe(std::move(callback)));
    }

    auto observe_scoped(functor_type callback) noexcept
    {
        return m_sp->subscribe(std::move(callback));
    }

    void notify() const
    {
        m_sp->invoke();
    }

    friend void swap(shared_observable<void> & lhs, shared_observable<void> & rhs)
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
        m_lock(unique_lock(*observable_ref.m_sp)),
        m_sp(observable_ref.m_sp)
    {
    }

    ~unique_txn()
    {
        if (m_sp)
        {
            // Downgrade to shared lock to avoid dealocking in callbacks
            m_lock.unlock_unique();

            // Notify callbacks with current value
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
    mutable switch_lock<observable_type> m_lock;
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
        m_lock(shared_lock(*observable_ref.m_sp)),
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
    mutable shared_lock<observable_type> m_lock;
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

/// \brief      Waits for modfication of `shared_observable` to meet the `predicate`.
/// \param      o               `shared_observable` to observe.
/// \param      predicate       Functor with signature `bool(const T & ... args)`; should return `true`
///                             if condition is met, `false` otherwise.
template<typename F, typename ... A>
void await_if(shared_observable<A ...> o, F && predicate)
{
    std::mutex mutex;
    std::condition_variable cv;
    bool awaited = false;

    auto observer = [&predicate, &mutex, &cv, &awaited](const A & ... args)
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

/// \brief      Waits for any modfication of `observable`.
template<typename ... A>
void await_any(const shared_observable<A ...> & o)
{
    auto predicate = [](const A & ... args)
    {
        return true;
    };

    await_if(o, std::move(predicate));
}

/// \brief      Waits for modfication of `shared_observable` to `value`.
template<typename ... A>
void await(const shared_observable<A ...> & o, const A & ... values)
{
    std::tuple awaited_values{values ...};

    auto predicate = [&awaited_values](const A & ... values)
    {
        return std::tuple{values ...} == awaited_values;
    };

    await_if(o, std::move(predicate));
}

/// \brief      Joins `shared_observable`s value as `functor`'s arguments.
template<typename F, typename Ta, typename Tb>
void join(F && functor, shared_observable<Ta> & a, shared_observable<Tb> & b)
{
    auto observer_a = [functor, b](auto ... args) mutable
    {
        std::tuple<Ta> args_a = {args ...};
        std::tuple<Tb> args_b = shared_txn{b};

        std::apply(functor, std::tuple_cat(args_a, args_b));
    };

    auto observer_b = [functor, a](auto ... args) mutable
    {
        std::tuple<Ta> args_a = shared_txn{a};
        std::tuple<Tb> args_b = {args ...};

        std::apply(functor, std::tuple_cat(args_a, args_b));
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));
}

template<typename F, typename Ta, typename Tb, typename Tc>
void join(F && functor, shared_observable<Ta> & a, shared_observable<Tb> & b, shared_observable<Tc> & c)
{
    auto observer_a = [functor, b, c](auto ... args) mutable
    {
        std::tuple<Ta> args_a = {args ...};
        std::tuple<Tb> args_b = b;
        std::tuple<Tc> args_c = c;

        std::apply(functor, std::tuple_cat(args_a, args_b, args_c));
    };

    auto observer_b = [functor, a, c](auto ... args) mutable
    {
        std::tuple<Ta> args_a = a;
        std::tuple<Tb> args_b = {args ...};
        std::tuple<Tc> args_c = c;

        std::apply(functor, std::tuple_cat(args_a, args_b, args_c));
    };

    auto observer_c = [functor, a, b](auto ... args) mutable
    {
        std::tuple<Ta> args_a = shared_txn{a};
        std::tuple<Tb> args_b = shared_txn{b};
        std::tuple<Tc> args_c = {args ...};

        std::apply(functor, std::tuple_cat(args_a, args_b, args_c));
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));
    c.observe(std::move(observer_c));
}

/// \brief      Joins `shared_observable`s into composite.
/// \warning    Changes made on composite do not propagate into the original `shared_observable`s.
template<typename Ta, typename Tb>
auto join(shared_observable<Ta> & a, shared_observable<Tb> & b)
{
    shared_observable<Ta, Tb> composite;

    auto observer_a = [composite, b](auto ... args) mutable
    {
        std::tuple<Ta> args_a = {args ...};
        std::tuple<Tb> args_b = shared_txn{b};

        unique_txn{composite} = std::tuple_cat(args_a, args_b);
    };

    auto observer_b = [composite, a](auto ... args) mutable
    {
        std::tuple<Ta> args_a = shared_txn{a};
        std::tuple<Tb> args_b = {args ...};

        unique_txn{composite} = std::tuple_cat(args_a, args_b);
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));

    return composite;
}