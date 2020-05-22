#pragma once

#include <mutex>
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
        std::unique_lock<std::mutex> lock(other.m_mutex);
        m_queue = other.m_queue;
    }

    atomic_queue(atomic_queue && other) noexcept
    {
        std::unique_lock<std::mutex> lock(other.m_mutex);
        m_queue = std::move(other.m_queue);
    }

    void push(const T & element)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(element);
    }

    void push(T && element)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(std::move(element));
    }

    template<typename ... A>
    void emplace(A && ... args)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.emplace(std::forward<A>(args) ...);
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        auto element = std::move(m_queue.front());
        m_queue.pop();
        return element;
    }

    bool empty() const
    {
        return m_queue.empty();
    }

    auto size() const
    {
        return m_queue.size();
    }

    friend void swap(atomic_queue & lhs, atomic_queue & rhs)
    {
        std::unique_lock<std::mutex> lock_rhs(rhs.m_mutex);
        std::unique_lock<std::mutex> lock_lhs(lhs.m_mutex);

        using std::swap;

        swap(lhs.m_queue, rhs.m_queue);
    }

private:
    mutable std::mutex m_mutex;

    std::queue<T> m_queue;
};