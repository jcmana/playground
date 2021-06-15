#pragma once

#include <thread>

/// \brief      Joining thread minimal implementation for executors.
class basic_executor
{
public:
    template<typename F, typename ... A>
    basic_executor(F && functor, A ... args) :
        m_thread(std::move(functor), std::forward<A>(args) ...)
    {
    }

    basic_executor(basic_executor && other) = default;

    ~basic_executor()
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

    basic_executor & operator  =(basic_executor && other) = default;

private:
    std::thread m_thread;
};