#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

template<typename T>
class executor_queue
{
public:
    executor_queue()
    {
    }

    executor_queue(const executor_queue & other)
    {
        std::unique_lock<std::mutex> lock(other.m_mutex);
        m_queue = other.m_queue;
    }

    executor_queue(executor_queue && other) noexcept :
        executor_queue()
    {
        std::unique_lock<std::mutex> lock(other.m_mutex);
        using std::swap;
        swap(*this, other);
    }

    executor_queue & operator  =(const executor_queue & other)
    {
        std::unique_lock<std::mutex> lock(other.m_mutex);
        m_queue = other.m_queue;

        return (*this);
    }

    executor_queue & operator  =(executor_queue && other) noexcept
    {
        std::unique_lock<std::mutex> lock(other.m_mutex);
        using std::swap;
        swap(*this, executor_queue());
        swap(*this, other);

        return (*this);
    }

    void push(const T & element)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(element);
        m_cv.notify_one();
    }

    void push(T && element)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(std::move(element));
        m_cv.notify_one();
    }

    template<typename ... A>
    void emplace(A && ... args)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.emplace(std::forward<A>(args) ...);
        m_cv.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        while (m_queue.empty())
        {
            m_cv.wait(lock);
        }

        auto element = std::move(m_queue.front());
        m_queue.pop();
        
        return element;
    }

    friend void swap(executor_queue & lhs, executor_queue & rhs)
    {
        std::unique_lock<std::mutex> lock_rhs(rhs.m_mutex);
        std::unique_lock<std::mutex> lock_lhs(lhs.m_mutex);

        using std::swap;
        swap(lhs.m_queue, rhs.m_queue);
    }

private:
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cv;

    std::queue<T> m_queue;
};
