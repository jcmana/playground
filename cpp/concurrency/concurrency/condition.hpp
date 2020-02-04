#pragma once

#include <mutex>
#include <condition_variable>
#include <utility>

template<typename T>
class condition
{
public:
    explicit condition() :
        m_value(T())
    {
    }

    explicit condition(const T & value) noexcept :
        m_value(value)
    {
    }

    explicit condition(T && value) noexcept :
        m_value(std::move(value))
    {
    }

    /// \brief      Assigns conditional value.
    ///
    /// Leads to unblocking all waiting threads and blocking until
    /// assigned value is handled by those.
    condition & operator  =(const T & value) noexcept
    {
        std::unique_lock<std::mutex> lock(m_value_mutex);

        m_value = value;
        m_value_cv.notify_all();

        while (is_handled() == false)
        {
            m_counter_cv.wait(lock);
        }

        return (*this);
    }

    /// \brief      Assigns conditional value.
    /// 
    /// Leads to unblocking all waiting threads and blocking until
    /// assigned value is handled by those.
    condition & operator  =(T && value) noexcept
    {
        // Critical section:
        {
            std::unique_lock<std::mutex> lock(m_value_mutex);
            m_value = std::move(value);
        }

        m_value_cv.notify_all();

        // Critical section:
        {
            std::unique_lock<std::mutex> lock(m_counter_mutex);

            while (is_handled() == false)
            {
                m_counter_cv.wait(lock);
            }
        }

        return (*this);
    }

    /// \brief      Blocks until conditional value is changed.
    T wait()
    {
        std::unique_lock<std::mutex> lock(m_value_mutex);
        const auto value = m_value;
        
        while (is_changed(value) == false)
        {
            // Critical section:
            {
                std::unique_lock<std::mutex> lock(m_counter_mutex);
                m_counter++;
            }

            m_value_cv.wait(lock);

            // Critical section:
            {
                std::unique_lock<std::mutex> lock(m_counter_mutex);
                m_counter--;
                m_counter_cv.notify_all();
            }
        }

        return m_value;
    }

    /*
    /// \brief      Blocks until conditional value is matched.
    T wait(const T & value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        while (is_matched(value) == false)
        {
            m_cv.wait(lock);
        }

        return m_value;
    }
    */

private:
    bool is_changed(const T & value) const
    {
        return (m_value != value);
    }

    bool is_matched(const T & value) const
    {
        return (m_value == value);
    }

    bool is_handled() const
    {
        return (m_counter == 0);
    }

private:
    mutable std::mutex m_value_mutex;
    mutable std::condition_variable m_value_cv;

    /// \brief      Conditional value.
    T m_value;

    mutable std::mutex m_counter_mutex;
    mutable std::condition_variable m_counter_cv;

    /// \brief      Counter of threads handling the notification.
    unsigned int m_counter = 0;
};