#pragma once

#include <memory>
#include <utility>
#include <optional>

#include "atomic_queue.hpp"

/// \brief      Executes posted tasks asynchronously in order.
template<typename F>
class executor
{
public:
    /// \brief      Default constructor, initializes executor ready to receive tasks.
    executor()
    {
        executor_pair ep;
        ep.thread = std::thread(&executor::thread_procedure, this);

        m_up = std::make_unique<executor_pair>(std::move(ep));
    }

    ~executor()
    {
        if (m_up)
        {
            m_up->queue.push(std::nullopt);
            m_up->queue.push(std::nullopt);
            m_up->thread.join();
        }
    }

    executor(executor && other) = default;

    executor & operator  =(executor && other) = default;

    /// \brief      Posts task into queue for asynchronous execution.
    void post(F task)
    {
        m_up->queue.push(std::move(task));
    }

private:
    struct executor_pair
    {
        // Synchronization FIFO for passing incoming tasks from issuing thread
        // to the worker thread = the executor.
        //
        // Tasks are optional to allow orderly termination of the executor, nullopt
        // task is a terminating task.
        atomic_queue<std::optional<F>> queue;

        // Executor thread
        std::thread thread;
    };

private:
    void thread_procedure()
    {
        while (true)
        {
            // Retrieve task from reader-writer queue
            auto optional_task = m_up->queue.pop();

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
    std::unique_ptr<executor_pair> m_up;
};
