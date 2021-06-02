#pragma once

#include <thread>
#include <future>
#include <memory>

#include "executor_queue.hpp"

class executor_thread
{
public:
    executor_thread() :
        m_thread()
    {
    }

    template<typename F, typename ... A>
    executor_thread(F && functor, A ... args) :
        m_thread(std::move(functor), std::forward<A>(args) ...)
    {
    }

    ~executor_thread()
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

    bool joinable() const
    {
        return m_thread.joinable();
    }

private:
    std::thread m_thread;
};