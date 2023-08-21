#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

template<typename T>
class atomic_queue
{
public:
    atomic_queue()
    {
    }

    atomic_queue(const atomic_queue & other)
    {
        std::unique_lock lock(other.m_mutex);
        m_queue = other.m_queue;
    }

    atomic_queue(atomic_queue && other) noexcept :
        atomic_queue()
    {
        swap(*this, other);
    }

    atomic_queue & operator  =(const atomic_queue & other)
    {
        std::unique_lock lock(other.m_mutex);
        m_queue = other.m_queue;

        return (*this);
    }

    atomic_queue & operator  =(atomic_queue && other) noexcept
    {
        auto empty = atomic_queue();

        swap(*this, empty);
        swap(*this, other);

        return (*this);
    }

    void push(T element)
    {
        std::unique_lock lock(m_mutex);
        m_queue.push(std::move(element));
        m_cv.notify_one();
    }

    template<typename ... A>
    void emplace(A && ... args)
    {
        std::unique_lock lock(m_mutex);
        m_queue.emplace(std::forward<A>(args) ...);
        m_cv.notify_one();
    }

    T pop()
    {
        std::unique_lock lock(m_mutex);

        while (m_queue.empty())
        {
            m_cv.wait(lock);
        }

        auto element = std::move(m_queue.front());
        m_queue.pop();
        
        return std::move(element);
    }

    friend void swap(atomic_queue & lhs, atomic_queue & rhs)
    {
        std::unique_lock lock_rhs(rhs.m_mutex);
        std::unique_lock lock_lhs(lhs.m_mutex);

        using std::swap;
        swap(lhs.m_queue, rhs.m_queue);
    }

private:
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cv;

    std::queue<T> m_queue;
};
