#include <iostream>

#include <thread>
#include <chrono>
#include <tuple>

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

        auto callback_pair = make_atomic_callback(i);
        auto callback = std::move(std::get<0>(callback_pair));
        auto callback_guard = std::move(std::get<1>(callback_pair));

        callback.invoke(&callback_intf::method);
    }

    // Multi-thread atomic_callback_store test:
    if (false)
    {
        callback_intf i;

        atomic_callback<callback_intf> callback;
        atomic_callback_guard<callback_intf> callback_guard;

        std::tie(callback, callback_guard) = make_atomic_callback(i);

        std::thread t([&]
        {
            auto g = std::move(callback_guard);
            callback.invoke(&callback_intf::method_slow);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            callback.invoke(&callback_intf::method_slow);

            auto g_move = std::move(g);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            callback.invoke(&callback_intf::method_slow);
        });

        auto c_move = std::move(callback);
        //callback = std::move(c_move);

        t.join();
    }

    if (true)
    {
        callback_intf i;
        atomic_callback_store<callback_intf> cs;
        cs.invoke(&callback_intf::method);
        auto cg = cs.subscribe(i);
        cs.invoke(&callback_intf::method);
        cs.invoke(&callback_intf::method_slow);
        auto ch = cs.subscribe(i);
        cs.invoke(&callback_intf::method);
        cs.invoke(&callback_intf::method_slow);
    }
}