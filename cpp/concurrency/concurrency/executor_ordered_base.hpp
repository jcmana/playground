#pragma once

#include <future>
#include <mutex>
#include <thread>
#include <queue>

template<typename T>
class executor_ordered_base
{
public:
    executor_ordered_base()
    {
    }

    executor_ordered_base(executor_ordered_base && other) noexcept :
        executor_ordered_base()
    {
        swap(*this, other);
    }

    executor_ordered_base & operator  =(executor_ordered_base && other) noexcept
    {
        swap(*this, executor_ordered_base());
        swap(*this, other);

        return (*this);
    }

    friend void swap(executor_ordered_base & lhs, executor_ordered_base & rhs)
    {
        std::unique_lock<std::mutex> lhs_lock(lhs.m_mutex);
        std::unique_lock<std::mutex> rhs_lock(rhs.m_mutex);

        using std::swap;

        swap(lhs.m_queue, rhs.m_queue);
    }

    void push(std::packaged_task<T()> task)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        // Push the new task to the queue
        m_queue.emplace(std::move(task));

        // Notify worker thread new task is available
        m_cv.notify_one();
    }

    std::packaged_task<T()> pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        while (m_queue.empty())
        {
            m_cv.wait(lock);
        }

        std::packaged_task<T()> task;
        task = std::move(m_queue.front());
        m_queue.pop();

        return task;
    }

private:
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cv;

    std::queue<std::packaged_task<T()>> m_queue;
};