#pragma once

#include <shared_mutex>
#include <functional>
#include <memory>

#include "../../callback_store/atomic_callback_store/atomic_callback_store.hpp"

/// \brief      Observable value, issues notifications after each modification.
template<typename T>
class observable
{
public:
    using value_type = T;
    using store_callback_type = std::function<void(const T &)>;
    using store_type = atomic_callback_store<store_callback_type>;
    using guard_type = atomic_callback_guard<store_callback_type>;

public:
    class modifier
    {
    public:
        modifier(observable & o) :
            m_lock(o.m_mutex),
            m_mref(o)
        {
        }

        operator T & ()
        {
            return get();
        }

        T & get()
        {   
            return m_mref.m_value;
        }

        ~modifier()
        {
            m_mref.m_store.invoke(m_mref.m_value);
        }

    private:
        std::unique_lock<std::shared_mutex> m_lock;
        observable & m_mref;
    };

    class accessor
    {
    public:
        accessor(const observable & o) :
            m_lock(o.m_mutex),
            m_cref(o)
        {
        }

        operator const T & () const
        {
            return get();
        }

        const T & get() const
        {   
            return m_cref.m_value;
        }

        ~accessor()
        {
        }

    private:
        std::shared_lock<std::shared_mutex> m_lock;
        const observable & m_cref;
    };

    friend class modifier;
    friend class accessor;

public:
    /// \brief      Default contructor.
    observable() :
        m_value()
    {
    }

    /// \brief      Constructor, initializes observable with `value`.
    observable(T value) :
        m_value(std::move(value))
    {
    }

    observable(observable && other) noexcept :
        observable()
    {
        swap(*this, other);
    }

    observable & operator  =(observable && other) noexcept
    {
        observable empty;
        swap(*this, empty);
        swap(*this, other);

        return (*this);
    }

    /// \brief      Subscribes `callback` for modification notifications.
    template<typename F>
    auto observe(F callback) const
    {
        return m_store.subscribe(std::move(callback));
    }

    void set(T value)
    {
        std::unique_lock lock(m_mutex);
        m_value = std::move(value);
        m_store.invoke(m_value);
    }

    const T & get() const
    {
        std::shared_lock lock(m_mutex);
        return m_value;
    }

    /// \brief      Modifies value with `functor`.
    /// \param      functor     Functor with signature `void(T &)`.
    template<typename F>
    void modify(F && functor)
    {
        std::unique_lock lock(m_mutex);
        functor(static_cast<T &>(m_value));
        m_store.invoke(m_value);
    }

    /// \brief      Reads value with `functor`.
    /// \param      functor     Functor with signature `void(const T &)`.
    template<typename F>
    void access(F && functor) const
    {
        std::shared_lock lock(m_mutex);
        functor(static_cast<const T &>(m_value));
    }

    /// \brief      Write-lock guard for the `observable`.
    modifier modify()
    {
        return modifier(*this);
    }

    /// \brief      Read-lock guard for the `observable`.
    accessor access() const
    {
        return accessor(*this);
    }

    /// \brief      Triggers subscribed callbacks as if modified.
    void trigger() const
    {
        m_store.invoke(m_value);
    }

    /// \brief      Swaps `lhs` and `rhs` `observable`s.
    template<typename T>
    friend void swap(observable<T> & lhs, observable<T> & rhs);

private:
    mutable std::shared_mutex m_mutex;
    mutable atomic_callback_store<std::function<void(const T &)>> m_store;

    T m_value;
};

template<typename T>
void swap(observable<T> & lhs, observable<T> & rhs)
{
    std::unique_lock lock_lhs(lhs.m_mutex);
    std::unique_lock lock_rhs(rhs.m_mutex);

    using std::swap;
    swap(lhs.m_store, rhs.m_store);
    swap(lhs.m_value, rhs.m_value);
}

/// \brief      Shared `observable` to be passed shared across concurrent threads.
template<typename T>
class shared_observable
{
public:
    shared_observable() :
        m_sp(new observable<T>)
    {
    }

    template<typename F>
    auto observe(F callback) const
    {
        return m_sp->observe(std::move(callback));
    }

    void set(T value)
    {
        m_sp->set(std::move(value));
    }

    const T & get() const
    {
        return m_sp->get();
    }

    // JMTODO: wrap rest of the observable interface

private:
    const std::shared_ptr<observable<T>> m_sp;
};

