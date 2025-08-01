#pragma once

#include <mutex>
#include <atomic>
#include <memory>

template<typename T>
class shared_atomic;

// Design questions:
// - does it have to implement BasicLockable
// - copy/move API or use transaction system from shared observable? both:
//     - copy/move when entire value is read/written, no access to reference 
//     - transactions when you need to work with a reference, accessible 
//       only through transaction

template<typename T>
class shared_atomic
{
public:
    using value_type = T;

public:
    shared_atomic() :
        shared_atomic(T())
    {
    }

    shared_atomic(T value) :
        m_sp(new T(value))
    {
    }

    shared_atomic & operator  =(T value)
    {
        std::unique_lock lock(m_mutex);
        (*m_sp) = value;
        return (*this);
    }

    T get() const
    {
        std::unique_lock lock(m_mutex);
        return (*m_sp);
    }

public:
    // Mutex concept implementation:
    void lock() const
    {
        m_mutex.lock();
    }

    void unlock() const
    {
        m_mutex.unlock();
    }

    bool try_lock() const
    {
        return m_mutex.try_lock();
    }

private:
    mutable std::mutex m_mutex;
    std::shared_ptr<T> m_sp;
};

/*
template<>
class shared_atomic<bool>
{
public:
    shared_atomic() :
        shared_atomic(bool())
    {
    }

    shared_atomic(bool value) :
        m_sp(new std::atomic<bool>(value))
    {
    }

    bool get() const
    {

    }

private:
    std::shared_ptr<std::atomic<bool>> m_sp;
};
*/