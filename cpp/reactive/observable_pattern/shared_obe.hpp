#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "basic_obe.hpp"

template<typename T>
class shared_obe;

/// \brief      Shared reference to an observable value.
template<typename T>
class shared_obe
{
public:
    template<typename TT>
    using functor_type = std::function<void(TT)>;
    using functor_void_type = std::function<void()>;
    using observable_type = basic_obe<functor_type, T>;
    using guard_type = typename observable_type::guard_type;
    using value_type = typename observable_type::value_type;
    using mutex_type = typename observable_type::mutex_type;

    template<typename TT>
    friend class unique_txn;

    template<typename TT>
    friend class shared_txn;

public:
    /// \brief      Constructor, creates shared observable with default value.
    shared_obe() noexcept;

    /// \brief      Copy constructor, copies shared observable, clears observers.
    shared_obe(const shared_obe & other) noexcept;

    /// \brief      Move constructor, moves shared observable, clears observers.
    shared_obe(shared_obe && other) noexcept;

    /// \brief      Constructor, initializes with copy of `value`.
    explicit shared_obe(T value) noexcept;

    shared_obe & operator  =(const shared_obe & other) noexcept;
    shared_obe & operator  =(shared_obe && other) noexcept;

    /// \brief      Observe changes with functor.
    void observe(functor_type<T> callback) noexcept;
    auto observe_scoped(functor_type<T> callback) noexcept;

    /// \brief      Observe changes with member function.
    template<typename C>
    void observe(void(C:: * f)(const T &), C * ptr);
    template<typename C>
    auto observe_scoped(void(C:: * f)(const T &), C * ptr);

    /// \brief      Observe changes with functor ignoring the value.
    void observe(functor_void_type callback);
    auto observe_scoped(functor_void_type callback);

    template<typename TT>
    friend void swap(shared_obe<TT> & lhs, shared_obe<TT> & rhs) noexcept;

private:
    std::shared_ptr<observable_type> m_sp;
    std::vector<guard_type> m_observers;
};

/// \brief      Shared reference to a no-value observable.
template<>
class shared_obe<void>
{
public:
    using functor_type = std::function<void()>;
    using observable_type = atomic_callback_store<functor_type>;
    using guard_type = atomic_callback_guard<functor_type>;

public:
    shared_obe() noexcept;
    shared_obe(const shared_obe & other) noexcept;
    shared_obe(shared_obe && other) noexcept;

    shared_obe & operator  =(const shared_obe & other) noexcept;
    shared_obe & operator  =(shared_obe && other) noexcept;

    void observe(functor_type callback) noexcept;
    auto observe_scoped(functor_type callback) noexcept;

    template<typename C>
    void observe(void(C:: * f)(), C * ptr);
    template<typename C>
    auto observe_scoped(void(C:: * f)(), C * ptr);

    void notify() const;

    friend void swap(shared_obe<void> & lhs, shared_obe<void> & rhs) noexcept;

private:
    std::shared_ptr<observable_type> m_sp;
    std::vector<guard_type> m_observers;
};

#pragma region shared_obe implementation:

template<typename T>
shared_obe<T>::shared_obe() noexcept :
    m_sp(new observable_type()),
    m_observers()
{
}

template<typename T>
shared_obe<T>::shared_obe(const shared_obe & other) noexcept :
    m_sp(other.m_sp),
    m_observers()
{
}

template<typename T>
shared_obe<T>::shared_obe(shared_obe && other) noexcept :
    shared_obe()
{
    swap(*this, other);
}

template<typename T>
shared_obe<T>::shared_obe(T value) noexcept :
    m_sp(new observable_type(std::move(value))),
    m_observers()
{
}

template<typename T>
shared_obe<T> &
shared_obe<T>::operator  =(const shared_obe & other) noexcept
{
    m_sp = other.m_sp;
    return (*this);
}

template<typename T>
shared_obe<T> &
shared_obe<T>::operator  =(shared_obe && other) noexcept
{
    auto empty = shared_obe();
    swap(*this, empty);
    swap(*this, other);
    return (*this);
}

template<typename T>
void 
shared_obe<T>::observe(functor_type<T> callback) noexcept
{
    m_observers.push_back(m_sp->observe(std::move(callback)));
}

template<typename T>
auto 
shared_obe<T>::observe_scoped(functor_type<T> callback) noexcept
{
    return m_sp->observe(std::move(callback));
}

template<typename T>
template<typename C>
void 
shared_obe<T>::observe(void(C:: * f)(const T &), C * ptr)
{
    m_observers.push_back(m_sp->observe(std::bind(f, ptr, std::placeholders::_1)));
}

template<typename T>
template<typename C>
auto 
shared_obe<T>::observe_scoped(void(C:: * f)(const T &), C * ptr)
{
    return m_sp->observe(std::bind(f, ptr, std::placeholders::_1));
}

template<typename T>
void 
shared_obe<T>::observe(functor_void_type callback)
{
    auto callback_wrapper = [callback](const T & value)
    {
        callback();
    };
    m_observers.push_back(m_sp->observe(std::move(callback_wrapper)));
}

template<typename T>
auto 
shared_obe<T>::observe_scoped(functor_void_type callback)
{
    auto callback_wrapper = [callback](const T & value)
    {
        callback();
    };
    return m_sp->observe(std::move(callback_wrapper));
}

template<typename T>
void 
swap(shared_obe<T> & lhs, shared_obe<T> & rhs) noexcept
{
    using std::swap;
    swap(lhs.m_sp, rhs.m_sp);

    lhs.m_observers.clear();
    rhs.m_observers.clear();
}

shared_obe<void>::shared_obe() noexcept :
    m_sp(new observable_type),
    m_observers()
{
}

inline shared_obe<void>::shared_obe(const shared_obe & other) noexcept :
    m_sp(other.m_sp),
    m_observers()
{
}

inline shared_obe<void>::shared_obe(shared_obe && other) noexcept :
    shared_obe()
{
    swap(*this, other);
}

inline shared_obe<void> & shared_obe<void>::operator  =(const shared_obe & other) noexcept
{
    m_sp = other.m_sp;
    return (*this);
}

inline shared_obe<void> & shared_obe<void>::operator  =(shared_obe && other) noexcept
{
    auto empty = shared_obe();
    swap(*this, empty);
    swap(*this, other);
    return (*this);
}

inline void shared_obe<void>::observe(functor_type callback) noexcept
{
    m_observers.push_back(m_sp->subscribe(std::move(callback)));
}

inline auto shared_obe<void>::observe_scoped(functor_type callback) noexcept
{
    return m_sp->subscribe(std::move(callback));
}

template<typename C>
inline void shared_obe<void>::observe(void(C:: * f)(), C * ptr)
{
    m_observers.push_back(m_sp->subscribe(std::move(std::bind(f, ptr))));
}

template<typename C>
inline auto shared_obe<void>::observe_scoped(void(C:: * f)(), C * ptr)
{
    return m_sp->subscribe(std::move(std::bind(f, ptr)));
}

inline void shared_obe<void>::notify() const
{
    m_sp->invoke();
}

inline void swap(shared_obe<void> & lhs, shared_obe<void> & rhs) noexcept
{
    using std::swap;
    swap(lhs.m_sp, rhs.m_sp);

    lhs.m_observers.clear();
    rhs.m_observers.clear();
}

#pragma endregion
