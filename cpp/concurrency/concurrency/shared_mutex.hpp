#pragma once

#include <mutex>
#include <shared_mutex>
#include <atomic>

class shared_mutex
{
public:
    // SharedMutex implementation:
    void lock()
    {
        m_shared.lock();
        m_unique.lock();
    }

    void unlock()
    {
        m_unique.unlock();
        m_shared.unlock();
    }

    void lock_shared()
    {
        m_shared.lock_shared();
    }

    void unlock_shared()
    {
        m_shared.unlock_shared();
    }

    void upgrade()
    {
        m_unique.lock();
    }

    void downgrade()
    {
        m_unique.unlock();
    }

private:
    std::mutex m_unique;
    std::shared_mutex m_shared;
};