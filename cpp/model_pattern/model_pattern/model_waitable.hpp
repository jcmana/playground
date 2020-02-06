#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>

/// \brief      Waitable concurrent modification.
class model_waitable
{
public:
    /// \brief      Blocks until this object's modification.
    void wait() const
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        const auto trigger_initial = m_trigger;
        while (m_trigger == trigger_initial)
        {
            m_cv.wait(lock);
        }
    }

    /// \brief      Trigger the modification.
    ///
    /// Waiting threads are woken up.
    void trigger()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_trigger++;
        m_cv.notify_all();
    }

private:
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cv;

    /// \brief      Represents the 'modification' condition.
    std::size_t m_trigger;
};
