#pragma once

#include <mutex>
#include <condition_variable>

class barrier
{
public:
    /// \brief      Default constructor.
    barrier() = default;

    ~barrier()
    {
        arrive_and_drop();
    }

    void arrive_and_wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        while (m_active)
        {
            m_cv.wait(lock);
        }
    }

    void arrive_and_drop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_active = false;
        m_cv.notify_all();
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;

    bool m_active = true;
};
