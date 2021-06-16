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
class executor
{
public:
    /// \brief      Default constructor, initializes executor ready to receive tasks.
    executor() :
        m_executor(&executor::thread_procedure, this)
    {
    }

    ~executor()
    {
        m_queue.push(std::function<T()>());
    }

    executor(const executor & other) = delete;
    
    executor(executor && other) noexcept :
        executor()
    {
        swap(*this, other);
    }
    executor & operator  =(const executor & other) = delete;

    executor & operator  =(executor && other) noexcept
    {
        swap(*this, executor());
        swap(*this, other);

        return (*this);
    }

    /// \brief      Posts tasks into queue for asynchronous execution.
    template<typename F, typename ... A>
    void post(F && f, A ... args)
    {
        m_queue.push(std::bind(f, std::forward<A>(args) ...));
    }

    friend void swap(executor<T> & lhs, executor<T> & rhs)
    {
        using std::swap;
        swap(lhs.m_queue, rhs.m_queue);
    }

private:
    void thread_procedure()
    {
        while (true)
        {
            auto task = m_queue.pop();

            if (task == false)
            {
                break;
            }

            task();
        }
    }

private:
    // Synchronization FIFO for passing incoming tasks from issuing thread
    // to the worker thread = the executor
    atomic_queue<std::function<T()>> m_queue;

    basic_executor m_executor;
};
