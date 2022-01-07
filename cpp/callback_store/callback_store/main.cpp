#include <iostream>

#include <thread>
#include <chrono>
#include <type_traits>

#include "callback_ref.hpp"
#include "callback_ref_listener.hpp"

#include "callback_store.hpp"
#include "callback_factory.hpp"

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

struct callback_intf_consumer
{
    void on_method()
    {
        std::cout << "callback_intf_consumer::on_method()" << std::endl;
    }

    void on_method_params(int n)
    {
        std::cout << "callback_intf_consumer::on_method_params() = " << n << std::endl;
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

void overload()
{
    std::cout << "overload()" << std::endl;
}

void overload(int)
{
    std::cout << "overload(int)" << std::endl;
}
void overload(int, double)
{
    std::cout << "overload(int, double)" << std::endl;
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

    // Callback default ctor, move, copy:
    if (false)
    {
        callback<void(*)()> cf;
        callback<callback_ref<callback_intf>> cr;

        callback<std::function<void()>> csf;
        callback_guard<std::function<void()>> g;

        std::tie(csf, g) = make_callback(std::function(function));

        // Copy, move
        //auto csf_copy = csf;        // deleted function
        auto csf_move = std::move(csf);

        // Check
        csf.invoke();
        csf_move.invoke();
    }

    // Callback using various functors:
    if (false)
    {
        {
            callback_intf i;
            auto [cb, cg] = make_callback(callback_ref(i));
            cb.invoke(&callback_intf::method_params, 13);
        }

        {
            auto [cb, cg] = make_callback(function);
            cb.invoke();
        }

        {
            auto [cb, cg] = make_callback(std::function(function));
            cb.invoke();
        }

        {
            auto [cb, cg] = make_callback(lambda);
            cb.invoke();
        }

        {
            callback_intf i;
            auto [cb, cg] = make_callback(std::bind(&callback_intf::method_params, &i, std::placeholders::_1));
            cb.invoke(7);
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

    // Callback store vector re-alloccation test:
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

    // Callback store inactive callback reuse:
    if (false)
    {
        callback_store<void(*)()> cs;

        decltype(cs.subscribe(function)) gp;

        {
            gp = cs.subscribe(function);
            auto gl = cs.subscribe(function);
            // gl is discarded here and callback becomes inactive
        }

        // inactive callback should be replaced here, instead of emplacing back
        auto gn = cs.subscribe(function);

        cs.invoke();
    }

    // Function callback_store test:
    if (false)
    {
        callback_store<void(*)()> s;
        auto g = s.subscribe(function);
        auto g_slow = s.subscribe(function_slow);

        s.invoke();
    }

    // callback_listener test:
    if (false)
    {
        callback_intf_consumer cic;

        callback_ref_listener<callback_intf, callback_intf_consumer> cl;
        cl.bind(&callback_intf::method, &callback_intf_consumer::on_method, cic);
        cl.bind(&callback_intf::method_params, &callback_intf_consumer::on_method_params, cic);

        auto [cb, cg] = make_callback(cl);
        cb.invoke(&callback_intf::method);
        cb.invoke(&callback_intf::method_params, 1337);
        cb.invoke(&callback_intf::method_slow);         // unbound method call is discarded
    }

    // Free function overloading:
    if (true)
    {
        auto [cb, cg] = make_callback(static_cast<void(*)()>(overload));
        cb.invoke();
    }
}
