#pragma once

#include <thread>
#include <future>

#include "executor_queue.hpp"

template<typename T>
class executor_thread
{
public:
    executor_thread()
    {
    }

    ~executor_thread()
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

private:
    void thread_procedure(executor_queue<std::packaged_task<T()>> & queue_ref)
    {
        while (true)
        {
            auto task = queue_ref.pop();

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
};