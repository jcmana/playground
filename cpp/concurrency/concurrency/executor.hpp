#pragma once

#include <memory>
#include <utility>
#include <optional>
#include <functional>

#include "atomic_queue.hpp"

/// \brief      Executes tasks asynchronously in FIFO order.
template<typename F>
class executor
{
public:
    /// \brief      Default constructor, initializes executor ready to receive tasks.
    executor()
    {
        m_up_queue = std::make_unique<atomic_queue<std::optional<F>>>();
        m_thread = std::thread(&executor::thread_procedure, std::ref(*m_up_queue));
    }

    /// \brief      Waits for all enqueued tasks and terminates worker thread.
    ~executor()
    {
        if (m_thread.joinable())
        {
            m_up_queue->push(std::nullopt);
            m_thread.join();
        }
    }

    /// \brief      Move constructor, transfers worker thread and task queue ownership.
    executor(executor && other) = default;

    /// \brief      Move assignment, transfers worker thread and task queue ownership.
    executor & operator  =(executor && other) = default;

    /// \brief      Posts task into queue for asynchronous execution.
    void post(F task)
    {
        m_up_queue->push(std::move(task));
    }

private:
    static void thread_procedure(atomic_queue<std::optional<F>> & queue)
    {
        while (true)
        {
            // Retrieve task from reader-writer queue
            auto optional_task = queue.pop();

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
    std::thread m_thread;

    // Queue is stored on heap to keep its address constant during move
    // operations to allow thread_procedure to keep working correctly.
    std::unique_ptr<atomic_queue<std::optional<F>>> m_up_queue;
};
