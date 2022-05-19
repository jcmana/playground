#include <mutex>
#include <future>
#include <array>

#include "spinlock.hpp"

int main()
{
    // lock-unlock pair
    if (false)
    {
        spinlock l;
        //std::mutex l;

        for (unsigned int n = 0; n < 100'000'000; n++)
        {
            l.lock();
            volatile char x = 0;
            l.unlock();
        }

        volatile char x = 0;
    }

    // basic concurrency
    if (true)
    {
        spinlock l;
        //std::mutex l;

        auto proc = [&]
        {
            for (unsigned int n = 0; n < 10'000'000; n++)
            {
                l.lock();
                volatile char x = 0;
                l.unlock();
            }

            volatile char x = 0;
        };

        {
            std::array<decltype(std::async(proc)), 10> futures;
            for (auto & f : futures)
            {
                f = std::async(proc);
            }
        }

        volatile char x = 0;
    }
}