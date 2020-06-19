#pragma once

#include <future>

template<typename T>
void wait_all(std::future<T> & future)
{
    if (future.valid())
    {
        future.wait();
    }
}

template<typename T, typename ... A>
void wait_all(std::future<T> & future, A & ... remaining)
{
    wait_all(future);
    wait_all(remaining ...);
}

template<typename C>
void wait_all(const C & range)
{
    for (const auto & future : range)
    {
        future.wait();
    }
}