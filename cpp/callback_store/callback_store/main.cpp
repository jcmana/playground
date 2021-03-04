#include <iostream>

#include <thread>
#include <chrono>
#include <type_traits>

#include "callback_ref.hpp"
#include "callback_store.hpp"
#include "callback_factory.hpp"
#include "callback_listener.hpp"

struct callback_intf
{
    void method()
    {
        std::cout << "callback_intf::method()" << std::endl;
    }

    void method_params(int n)
    {
        std::cout << "callback_intf::method_params()" << std::endl;
    }

    void method_slow()
    {
        std::cout << "callback_intf::method_slow()" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};

struct callback_abstract_intf
{
    virtual void method()
    {
    }

    virtual void method_slow()
    {
    }
};

void function()
{
    std::cout << "function()" << std::endl;
}

void function_slow()
{
    std::cout << "function_slow()" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

auto lambda = []
{
    std::cout << "lambda[]" << std::endl;
};

int main()
{
    // Core idea behind callback wrapper:
    if (false)
    {
        // Calling back to function
        {
            auto function_ptr = function;
            function_ptr();
        }

        // Calling back to method
        {
            callback_intf ci;
            auto method_ptr = &callback_intf::method;
            (ci.*method_ptr)();
        }

        // Calling back to functor
        {
            callback_intf ci;
            auto functor = std::bind(&callback_intf::method_slow, &ci);
            functor();
        }
    }

    // Callback using invocation wrapper:
    if (false)
    {

        {
            callback_intf i;
            callback_ref ri(i);
            auto [cb, cg] = make_callback(ri);
            cb.invoke(&callback_intf::method_params, 13);
        }

        {
            std::function f(function);
            auto [cb, cg] = make_callback(f);
            cb.invoke();
        }

        {
            callback_intf i;
            auto [cb, cg] = make_callback(std::bind(&callback_intf::method_params, &i, std::placeholders::_1));
            cb.invoke(7);
        }

        {
            auto [cb, cg] = make_callback(function);
            cb.invoke();
        }
    }

    // Simple callback_store test:
    if (false)
    {
        callback_intf cia;
        callback_intf cib;

        callback_store<callback_ref<callback_intf>> cs;
        cs.invoke(&callback_intf::method);

        {
            auto cg = cs.subscribe(cia);
            cs.invoke(&callback_intf::method);

            auto cg_move = std::move(cg);
            cs.invoke(&callback_intf::method);

            {
                auto cg = cs.subscribe(cib);
                cs.invoke(&callback_intf::method);

                cg = {};
                cs.invoke(&callback_intf::method_slow);
            }

            cs.invoke(&callback_intf::method);

            const auto & cs_const_ref = cs;
            cs.invoke(&callback_intf::method);
        }

        auto cs_move = std::move(cs);
        cs.invoke(&callback_intf::method);
        cs_move.invoke(&callback_intf::method);
    }

    // Callback vector re-alloccation test:
    if (false)
    {
        static constexpr std::size_t count = 100;

        std::vector<callback_intf> ci;

        callback_store<callback_ref<callback_intf>> cs;
        std::vector<callback_guard<callback_ref<callback_intf>>> cg;

        for (std::size_t n = 0; n < count; ++n)
        {
            auto it = ci.emplace(ci.end());
            cg.push_back(cs.subscribe(*it));
        }
    }


    // Function callback_store test:
    if (false)
    {
        callback_store<void(*)()> s;
        auto g = s.subscribe(function);
        auto g_slow = s.subscribe(function_slow);

        s.invoke();
    }

    /*
    // callback_listener test:
    if (true)
    {
        callback_intf ci;
        callback_listener<callback_abstract_intf, callback_intf> cl;
        cl.bind(&callback_abstract_intf::method, &callback_intf::method, ci);
        cl.invoke(&callback_abstract_intf::method);

        auto [cb, cg] = make_callback(cl);
        //auto cb = std::move(std::get<0>(cbpack));
        //auto cg = std::move(std::get<1>(cbpack));

        //cb.invoke(&callback_intf::method);
    }
    */

    return 0;
}
