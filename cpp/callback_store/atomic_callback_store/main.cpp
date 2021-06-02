#include <iostream>

#include <thread>
#include <chrono>
#include <tuple>

#include "atomic_callback.hpp"
#include "atomic_callback_store.hpp"

#include "../callback_store/callback_ref.hpp"

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

int main()
{
    // Simple atomic_callback_store test:
    if (false)
    {
        callback_intf i;

        auto [c, g] = make_atomic_callback(callback_ref(i));
        c.invoke(&callback_intf::method);
    }

    // Multi-thread atomic_callback test:
    if (false)
    {
        callback_intf i;

        atomic_callback<callback_ref<callback_intf>> callback;
        atomic_callback_guard<callback_ref<callback_intf>> callback_guard;

        std::tie(callback, callback_guard) = make_atomic_callback(callback_ref(i));

        auto proc = [&]
        {
            while (callback.active())
            {
                callback.invoke(&callback_intf::method_slow);
            }
        };

        std::thread t(proc);

        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // Depending on the scheduling of this thread and t, following instruction
        // disconnects the link and callback becomes inactive, therefore only limited
        // number of invokes will go through
        callback_guard = {};

        t.join();
    }

    // Multi-thread atomic_callback test (inlined):
    if (false)
    {
        std::shared_ptr<std::mutex> sp_mutex = std::make_shared<std::mutex>();

        auto proc = [&sp_mutex]
        {
            auto sp_mutex_local = sp_mutex;

            while (sp_mutex_local.use_count() == 2)
            {
                std::unique_lock lock(*sp_mutex_local);
                function_slow();
            }
        };

        std::thread t(proc);

        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // Depending on the scheduling of this thread and t, following instruction
        // releases the shared_ptr, therefore only limited number of invokes will go through
        {
            std::unique_lock lock(*sp_mutex);

            if (sp_mutex.use_count() == 2)
            {
                sp_mutex.reset();
            }
        }

        // This loops almost endlessly, most likely starvation caused by scheduler. There is a
        // race on mutex locking because function_slow() is called under lock.

        t.join();
    }

    if (false)
    {
        callback_intf i;
        atomic_callback_store<callback_ref<callback_intf>> cs;
        cs.invoke(&callback_intf::method);
        auto cg = cs.subscribe(i);
        cs.invoke(&callback_intf::method);
        cs.invoke(&callback_intf::method_slow);
        auto ch = cs.subscribe(i);
        cs.invoke(&callback_intf::method);
        cs.invoke(&callback_intf::method_slow);

        auto cs_moved = std::move(cs);
        cs.invoke(&callback_intf::method);
        cs_moved.invoke(&callback_intf::method);
    }

    if (true)
    {
        atomic_callback_guard<void> g;
        auto g_moved = std::move(g);
    }
}