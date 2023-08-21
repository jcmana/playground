#pragma once

#include <thread>

/// \brief      Joining thread minimal implementation for executors.
class basic_executor : std::thread
{
public:
    basic_executor()
    {
    }

    template<typename F, typename ... A>
    basic_executor(F && functor, A ... args) :
        std::thread(std::move(functor), std::forward<A>(args) ...)
    {
    }

    basic_executor(basic_executor && other) = default;

    ~basic_executor()
    {
        if (joinable())
        {
            join();
        }
    }

    basic_executor & operator  =(basic_executor && other) = default;
};