#pragma once

#include <mutex>
#include <shared_mutex>
#include <condition_variable>

class shared_mutex
{
public:
    shared_mutex()
    {
    }

    /// \brief      Upgrades the mutex lock from shared to unique.
    ///
    /// Expects mutex in `lock_shared()` state.
    void lock_unique()
    {
        std::unique_lock lock(m_mutex);
        while (m_shared_counter > 1 || m_unique_counter > 0)
        {
            m_cv.wait(lock);
        }

        m_unique_counter++;
    }

    /// \brief      Downgrades the mutex lock from unique to shared.
    ///
    /// Expects mutex in `lock()` state. 
    void unlock_unique()
    {
        m_unique_counter--;
        m_cv.notify_one();
    }

public:
    // SharedMutex implementation:
    void lock()
    {
        std::unique_lock lock(m_mutex);
        while (m_shared_counter > 0 || m_unique_counter > 0)
        {
            m_cv.wait(lock);
        }

        m_unique_counter++;
        m_shared_counter++;
    }

    void unlock()
    {
        m_unique_counter--;
        m_shared_counter--;
        m_cv.notify_one();
    }

    void lock_shared()
    {
        std::unique_lock lock(m_mutex);
        while (m_unique_counter > 0)
        {
            m_cv.wait(lock);
        }

        m_shared_counter++;
    }

    void unlock_shared()
    {
        {
            std::unique_lock lock(m_mutex);
            m_shared_counter--;
        }
        m_cv.notify_one();
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    unsigned int m_unique_counter = 0;
    unsigned int m_shared_counter = 0;
};

class unique_lock
{
public:
    unique_lock(shared_mutex & mutex) :
        m_mutex_ptr(&mutex)
    {
        m_mutex_ptr->lock();
    }

    ~unique_lock()
    {
        m_mutex_ptr->unlock();
    }

private:
    shared_mutex * m_mutex_ptr;
};

class shared_lock
{
public:
    shared_lock(shared_mutex & mutex) :
        m_mutex_ptr(&mutex)
    {
        m_mutex_ptr->lock_shared();
    }

    ~shared_lock()
    {
        m_mutex_ptr->unlock_shared();
    }

private:
    shared_mutex * m_mutex_ptr;
};

/*
class scoped_lock
{
public:
    enum class ownership
    {
        unique,
        shared,
    };

public:
    scoped_lock() :
        m_mutex_ptr(nullptr)
    {
    }

    scoped_lock(shared_mutex & mutex, ownership o = ownership::shared) :
        m_mutex_ptr(&mutex),
        m_ownership(o)
    {
        switch (m_ownership)
        {
            case ownership::unique:
                m_mutex_ptr->lock();
            case ownership::shared:
                m_mutex_ptr->lock_shared();
        }
    }

    ~scoped_lock()
    {
        if (m_mutex_ptr == nullptr)
        {
            return;
        }

        switch (m_ownership)
        {
            case ownership::unique:
                m_mutex_ptr->unlock();
            case ownership::shared:
                m_mutex_ptr->unlock_shared();
        }
    }

    void upgrade()
    {
        m_mutex_ptr->upgrade();
        m_ownership = ownership::unique;
    }

    void downgrade()
    {
        m_mutex_ptr->downgrade();
        m_ownership = ownership::shared;
    }

private:
    shared_mutex * m_mutex_ptr;
    ownership m_ownership;
};
*/