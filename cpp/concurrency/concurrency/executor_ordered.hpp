#pragma once

#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>

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
        push(std::move(task));

        m_thread.join();
    }

    /// \brief      Posts tasks into queue for asynchronous execution.
    template<typename F>
    std::future<void> post(F && f)
    {
        std::packaged_task<T()> task(f);
        auto future = task.get_future();
        push(std::move(task));

        return future;
    }

    friend void swap(executor_ordered & lhs, executor_ordered & rhs)
    {
        std::unique_lock<std::mutex> lhs_lock(lhs.m_mutex);
        std::unique_lock<std::mutex> rhs_lock(rhs.m_mutex);

        using std::swap;

        swap(lhs.m_queue, rhs.m_queue);
    }

private:
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

    void thread_procedure()
    {
        while (true)
        {
            std::packaged_task<T()> task = pop();

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

    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cv;

    std::queue<std::packaged_task<T()>> m_queue;
};
