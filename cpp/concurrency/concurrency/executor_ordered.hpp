#pragma once

#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "executor_queue.hpp"
#include "executor_ordered_base.hpp"

/// \brief      Executes posted tasks asynchronously in order.
template<typename T>
class executor_ordered
{
public:
    /// \brief      Default constructor, initializes executor ready to receive tasks.
    executor_ordered()
    {
        m_thread = std::thread(&executor_ordered::thread_procedure, this);
    }

    executor_ordered(const executor_ordered & other) = delete;
    
    executor_ordered(executor_ordered && other) noexcept :
        executor_ordered()
    {
        swap(*this, other);
    }

    executor_ordered & operator  =(const executor_ordered & other) = delete;

    executor_ordered & operator  =(executor_ordered && other) noexcept
    {
        swap(*this, executor_ordered());
        swap(*this, other);

        return (*this);
    }

    /// \brief      Destructor, blocks until posted tasks are finished.
    ~executor_ordered()
    {
        std::packaged_task<T()> task;
        m_queue.push(std::move(task));

        m_thread.join();
    }

    /// \brief      Posts tasks into queue for asynchronous execution.
    template<typename F>
    std::future<void> post(F && f)
    {
        std::packaged_task<T()> task(f);
        auto future = task.get_future();
        m_queue.push(std::move(task));

        return future;
    }

    friend void swap(executor_ordered & lhs, executor_ordered & rhs)
    {
        using std::swap;
        swap(lhs.m_queue, rhs.m_queue);
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
    std::thread m_thread;
    executor_queue<std::packaged_task<T()>> m_queue;
};
