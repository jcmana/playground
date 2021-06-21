#pragma once

#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <utility>
#include <optional>

#include "basic_executor.hpp"
#include "atomic_queue.hpp"

/// \brief      Executes posted tasks asynchronously in order.
template<typename F>
class executor
{
public:
    /// \brief      Default constructor, initializes executor ready to receive tasks.
    executor() :
        m_thread(&executor::thread_procedure, this)
    {
    }

    ~executor()
    {
        m_queue.push(std::nullopt);

        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

    executor(executor && other)
    {
        // Post thread terminating task

        // Swap queues

        // Restart threads
    }

    executor & operator  =(executor && other) = default;

    /// \brief      Posts task into queue for asynchronous execution.
    void post(F task)
    {
        m_queue.push(std::move(task));
    }

private:
    void thread_procedure()
    {
        while (true)
        {
            // Retrieve task from reader-writer queue
            auto optional_task = m_queue.pop();

            // Check for terminating task
            if (optional_task.has_value() == false)
            {
                break;
            }

            // Execute the task
            optional_task.value()();
        }
    }

private:
    // Synchronization FIFO for passing incoming tasks from issuing thread
    // to the worker thread = the executor.
    //
    // Tasks are optional to allow orderly termination of the executor, nullopt
    // task is a terminating task.
    atomic_queue<std::optional<F>> m_queue;

    // Executor thread
    std::thread m_thread;
};
