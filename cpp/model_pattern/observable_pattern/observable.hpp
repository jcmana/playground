#pragma once

#include <shared_mutex>
#include <functional>

#include "../../callback_store/atomic_callback_store/atomic_callback_store.hpp"
template<typename T>
class observable
{
public:
    using callback = std::function<void(const T &)>;

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
            return m_mref.m_value;
        }

        ~modifier()
        {
            m_mref.m_store.invoke(m_mref.m_value);
        }

    private:
        observable & m_mref;
        std::unique_lock<std::shared_mutex> m_lock;
    };

    class accessor
    {
    public:
        accessor(const observable & o) :
            m_lock(o.m_mutex),
            m_cref(o)
        {
        }

        operator const T & ()
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

public:
    auto observe(callback c)
    {
        return m_store.subscribe(std::move(c));
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

    template<typename F>
    void modify(F && functor)
    {
        std::unique_lock lock(m_mutex);
        functor(static_cast<T &>(m_value));
        m_store.invoke(m_value);
    }

    template<typename F>
    void access(F && functor) const
    {
        std::shared_lock lock(m_mutex);
        functor(static_cast<const T &>(m_value));
    }

    modifier modify()
    {
        return modifier(*this);
    }

    accessor access() const
    {
        return accessor(*this);
    }

private:
    mutable std::shared_mutex m_mutex;
    mutable atomic_callback_store<callback> m_store;

    T m_value;

    friend class modifier;
    friend class accessor;
};
