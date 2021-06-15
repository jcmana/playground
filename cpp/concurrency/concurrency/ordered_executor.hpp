#pragma once

#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <utility>

#include "basic_executor.hpp"
#include "atomic_queue.hpp"

/// \brief      Executes posted tasks asynchronously in order.
template<typename T>
class ordered_executor
{
public:
    /// \brief      Default constructor, initializes executor ready to receive tasks.
    ordered_executor() :
        m_executor(&ordered_executor::thread_procedure, this)
    {
    }

    ~ordered_executor()
    {
        m_queue.push(std::packaged_task<T()>());
    }

    ordered_executor(const ordered_executor & other) = delete;
    
    ordered_executor(ordered_executor && other) noexcept :
        ordered_executor()
    {
        swap(*this, other);
    }
    ordered_executor & operator  =(const ordered_executor & other) = delete;

    ordered_executor & operator  =(ordered_executor && other) noexcept
    {
        swap(*this, ordered_executor());
        swap(*this, other);

        return (*this);
    }

    /// \brief      Posts tasks into queue for asynchronous execution.
    template<typename F, typename ... A>
    std::future<T> post(F && f, A ... args)
    {
        auto binder = std::bind(f, std::forward<A>(args) ...);
        std::packaged_task<T()> task(std::move(binder));
        auto future = task.get_future();
        m_queue.push(std::move(task));

        return future;
    }

    friend void swap(ordered_executor<T> & lhs, ordered_executor<T> & rhs)
    {
        using std::swap;
        swap(lhs.m_queue, rhs.m_queue);
        //swap(lhs.m_executor, rhs.m_executor);
    }

private:
    void thread_procedure()
    {
        while (true)
        {
            auto task = m_queue.pop();

            if (task.valid() == false)
            {
                break;
            }

            // Execute the packaged task:
            // - runs the code itself
            // - sets the underlaying promise state
            // - sets the bound future value
            task();
        }
    }

private:
    atomic_queue<std::packaged_task<T()>> m_queue;
    basic_executor m_executor;
};
