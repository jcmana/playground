#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <vector>
#include <queue>
#include <functional>

#include "executor_queue.hpp"

template<typename T>
class executor_ordered_pool
{
public:
    executor_ordered_pool() :
        executor_ordered_pool(std::thread::hardware_concurrency())
    {
    }

    explicit executor_ordered_pool(unsigned int size) :
        m_pool(size)
    {
        for (auto & thread : m_pool)
        {
            thread = std::thread(&executor_ordered_pool::thread_procedure, this);
        }
    };

    executor_ordered_pool(const executor_ordered_pool & other) = delete;

    executor_ordered_pool(executor_ordered_pool && other) noexcept :
        executor_ordered_pool()
    {
        swap(*this, other);
    }

    executor_ordered_pool & operator  =(const executor_ordered_pool & other) = delete;

    executor_ordered_pool & operator  =(executor_ordered_pool && other) noexcept
    {
        swap(*this, executor_ordered_pool());
        swap(*this, other);

        return (*this);
    }

    ~executor_ordered_pool()
    {
        for (auto & thread : m_pool)
        {
            std::packaged_task<T()> task;
            m_queue.push(std::move(task));
        }

        for (auto & thread : m_pool)
        {
            thread.join();
        }
    }

    template<typename F>
    std::future<void> post(F && f)
    {
        std::packaged_task<T()> task(f);
        auto future = task.get_future();
        m_queue.push(std::move(task));

        return future;
    }

    void resize(unsigned int size)
    {
        const auto diff = int(size) - int(m_pool.size());

        // JMTODO: resizing logic
        // - how to stop some threads?
        // - how to join() those threads?
    }

    friend void swap(executor_ordered_pool & lhs, executor_ordered_pool & rhs)
    {
        using std::swap;
        swap(lhs.m_queue, rhs.m_queue);

        const auto size_lhs = lhs.size();
        const auto size_rhs = rhs.size();

        lhs.resize(size_rhs);
        rhs.resize(size_lhs);
    }

private:
    void thread_procedure()
    {
        while (true)
        {
            std::packaged_task<T()> task = m_queue.pop();

            if (task.valid() == false)
            {
                break;
            }

            try
            {
                task();
            }
            catch (...)
            {
            }
        }
    }

private:
    std::vector<std::thread> m_pool;
    executor_queue<std::packaged_task<T()>> m_queue;
};
