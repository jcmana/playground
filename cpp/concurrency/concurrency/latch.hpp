#pragma once

#include <cstddef>
#include <mutex>
#include <condition_variable>

class latch
{
public:
    explicit latch(std::ptrdiff_t value) :
        m_value(value)
    {
    }

    void count_down()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_value--;
    }

    void wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        while (m_value > 0)
        {
            m_cv.wait(lock);
        }
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;

    std::ptrdiff_t m_value;
};