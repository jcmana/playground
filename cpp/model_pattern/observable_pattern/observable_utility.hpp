#pragma once

#include <mutex>
#include <condition_variable>
#include <initializer_list>

#include "observable.hpp"

/// \brief      Waits for any modfication of `observable`.
template<typename T>
void await(observable<T> & o)
{
    std::mutex mutex;
    std::condition_variable cv;

    auto wait = [&](const T &)
    {
        cv.notify_one();
    };

    auto guard = o.observe(wait);
    
    // Critical section:
    {
        std::unique_lock lock(mutex);
        cv.wait(lock);
    }
}

/// \brief      Waits for modfication to `value` of `observable`.
template<typename T>
void await(observable<T> & o, const T & awaited_value)
{
    std::mutex mutex;
    std::condition_variable cv;

    auto wait = [&](const T & value)
    {
        if (value == awaited_value)
        {
            cv.notify_one();
        }
    };

    auto guard = o.observe(wait);

    // Critical section:
    {
        std::unique_lock lock(mutex);
        cv.wait(lock);
    }
}

/// \brief      Waits for modfication to `value` of `observable`.
template<typename T>
void await(observable<T> & o, std::initializer_list<T> awaited_list)
{
    std::mutex mutex;
    std::condition_variable cv;

    auto wait = [&](const T & value)
    {
        for (const auto awaited_value : awaited_list)
        {
            if (value == awaited_value)
            {
                cv.notify_one();
            }
        }
    };

    auto guard = o.observe(wait);

    // Critical section:
    {
        std::unique_lock lock(mutex);
        cv.wait(lock);
    }
}