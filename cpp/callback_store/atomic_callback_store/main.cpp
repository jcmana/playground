#include <iostream>

#include <thread>
#include <chrono>

#include "atomic_callback_store.hpp"

struct callback_intf
{
    void method()
    {
        std::cout << "callback_intf::method(): " << std::endl;
    }

    void method_slow()
    {
        std::cout << "callback_intf::method_slow()" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};

void function()
{
    std::cout << "function(): " << std::endl;
}

void function_slow()
{
    std::cout << "function_slow()" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void main()
{
    // Simple atomic_callback_store test:
    if (false)
    {
        callback_intf ci;

        atomic_callback<callback_intf> ac(&ci);
        atomic_callback_guard<callback_intf> acg(&ac);

        ac.invoke(&callback_intf::method);
    }

    // Multi-thread atomic_callback_store test:
    if (false)
    {
        callback_intf ci;

        atomic_callback<callback_intf> ac(&ci);

        std::thread ta([&]
        {
            atomic_callback_guard<callback_intf> acg(&ac);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });

        std::thread tb([&]
        {
            ac.invoke(&callback_intf::method_slow);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            ac.invoke(&callback_intf::method_slow);
        });

        auto ac_move = std::move(ac);
        ac = std::move(ac_move);

        ta.join();
        tb.join();
    }
}