#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "../../callback_store/atomic_callback_store/atomic_callback_store.hpp"

/// \brief      Shared reference to an event.
template<typename T>
class shared_evt;

/// \brief      Shared reference to an event of type `T`.
template<typename T>
class shared_evt
{
public:
    using functor_type = std::function<void(T)>;
    using functor_void_type = std::function<void()>;

    using store_type = typename atomic_callback_store<functor_type>;
    using guard_type = typename atomic_callback_guard<functor_type>;

public:
    /// \brief      Default constructor, creates un-observed event.
    shared_evt() :
        m_sp(new store_type()),
        m_observers()
    {
    }

    /// \brief      Copy constructor, clears observers.
    shared_evt(const shared_evt & other) :
        m_sp(other.m_sp),
        m_observers()
    {
    }

    /// \brief      Move constructor, clears observers.
    shared_evt(shared_evt && other) :
        shared_evt()
    {
        swap(*this, other);
    }

    auto observe_scoped(functor_type callback)
    {
        return m_sp->subscribe(std::move(callback));
    }

    void observe(functor_type callback)
    {
        m_observers.push_back(observe_scoped(std::move(callback)));
    }

    template<typename C>
    auto observe_scoped(void(C:: * f)(const T &), C * ptr)
    {
        return m_sp->subscribe(std::bind(f, ptr, std::placeholders::_1));
    }

    template<typename C>
    void observe(void(C:: * f)(const T &), C * ptr)
    {
        m_observers.push_back(observe_scoped(f, ptr));
    }

    void notify(const T & value) const
    {
        m_sp->invoke(value);
    }

    template<typename TT>
    friend void swap(shared_evt<TT> & lhs, shared_evt<TT> & rhs) noexcept
    {
        using std::swap;
        swap(lhs.m_sp, rhs.m_sp);

        lhs.m_observers.clear();
        rhs.m_observers.clear();
    }

private:
    std::shared_ptr<store_type> m_sp;
    std::vector<guard_type> m_observers;
};

/// \brief      Shared reference to an event of type `void`.
template<>
class shared_evt<void>
{
public:
    using functor_type = std::function<void()>;

    using store_type = typename atomic_callback_store<functor_type>;
    using guard_type = typename atomic_callback_guard<functor_type>;

public:
    shared_evt() :
        m_sp(new store_type()),
        m_observer()
    {
    }

    shared_evt(const shared_evt & other) :
        m_sp(other.m_sp),
        m_observer()
    {
    }

    shared_evt(shared_evt && other) :
        shared_evt()
    {
        swap(*this, other);
    }

    auto observe_scoped(functor_type callback)
    {
        return m_sp->subscribe(std::move(callback));
        //return atomic_callback_guard<functor_type>();
    }

    void observe(functor_type callback)
    {
        m_observer = observe_scoped(std::move(callback));
    }

    template<typename C>
    auto observe_scoped(void(C:: * f)(), C * ptr)
    {
        return m_sp->subscribe(std::bind_front(f, ptr));
        //return atomic_callback_guard<functor_type>();
    }

    template<typename C>
    void observe(void(C:: * f)(), C * ptr)
    {
        m_observer = observe_scoped(f, ptr);
    }

    void notify() const
    {
        m_sp->invoke();
    }

    friend void swap(shared_evt<void> & lhs, shared_evt<void> & rhs) noexcept
    {
        using std::swap;
        swap(lhs.m_sp, rhs.m_sp);

        lhs.m_observer = {};
        rhs.m_observer = {};
    }

private:
    std::shared_ptr<store_type> m_sp;
    guard_type m_observer;
};
