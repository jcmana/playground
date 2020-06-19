#pragma once

#include <thread>
#include <future>

#include "executor_queue.hpp"

template<typename T>
class executor_thread
{
public:
    enum class state
    {
        idle,
        execution,
        finished,
    };

public:
    executor_thread()
    {
    }

    explicit executor_thread(executor_queue<std::packaged_task<T()>> & queue) :
        m_queue_ref(queue)
    {
        m_thread = std::thread(std::bind(&executor_thread::thread_procedure, this));
    }

    /*
    executor_thread(executor_thread && other) noexcept :
        executor_thread
    {

    }
    */

    ~executor_thread()
    {
        std::packaged_task<T()> task;
        m_queue_ref.push(std::move(task));

        m_thread.join();
    }

    auto state() const
    {
        return m_state;
    }

private:
    void thread_procedure()
    {
        m_state = state::execution;

        while (true)
        {
            std::packaged_task<T()> task = m_queue_ref.pop();

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

        m_state = state::finished;
    }

private:
    enum class state m_state = state::idle;
    
    executor_queue<std::packaged_task<T()>> & m_queue_ref;

    std::thread m_thread;
};