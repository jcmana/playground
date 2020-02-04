#pragma once

#include <mutex>
#include <atomic>
#include <condition_variable>

class barrier
{
public:
    /// \brief      Default constructor.
    barrier() :
        m_active(true)
    {
    };

    barrier(const barrier & other) noexcept
    {

    }

    ~barrier()
    {
        arrive_and_drop();
    }

    void arrive_and_wait()
    {
        std::unique_lock<std::mutex> lock(m_write);

        while (m_active)
        {
            m_value_cv.wait(lock);
        }
    }

    void arrive_and_drop()
    {
        std::unique_lock<std::mutex> lock(m_write);

        m_active = false;
        m_value_cv.notify_all();
    }

private:
    std::mutex m_write;
    std::condition_variable m_value_cv;

    bool m_active;
};
