#include <iostream>

#include <thread>
#include <chrono>

#include "atomic_callback.hpp"
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
        callback_intf i;

        atomic_callback<callback_intf> c(i);
        atomic_callback_guard<callback_intf> g(c);

        c.invoke(&callback_intf::method);
    }

    // Multi-thread atomic_callback_store test:
    if (true)
    {
        callback_intf i;

        atomic_callback<callback_intf> c(i);

        std::thread t([&]
        {
            atomic_callback_guard<callback_intf> g(c);
            c.invoke(&callback_intf::method_slow);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            c.invoke(&callback_intf::method_slow);

            auto g_move = std::move(g);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            c.invoke(&callback_intf::method_slow);
        });

        auto c_move = std::move(c);
        c = std::move(c_move);

        t.join();
    }
}