#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <vector>
#include <queue>
#include <functional>

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

    ~executor_ordered_pool()
    {
        for (auto & thread : m_pool)
        {
            std::packaged_task<T()> task;
            push(std::move(task));
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
        push(std::move(task));

        return future;
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
    std::vector<std::thread> m_pool;

    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cv;

    std::queue<std::packaged_task<T()>> m_queue;
};