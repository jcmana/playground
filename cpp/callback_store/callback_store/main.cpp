#include <iostream>

#include <thread>
#include <chrono>
#include <type_traits>

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

// Function-like callback:
template<typename R, typename ... A>
struct flc;

template<typename R, typename ... A>
struct flc<R(A ...)>
{
    template<typename F>
    flc(F functor) :
        m_functor(std::move(functor))
    {
    }

    std::function<R(A ...)> m_functor;
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

    if (false)
    {
        flc<void()> c(function);

        auto lambda = []
        {
            std::cout << "holyfuck lambda" << std::endl;
        };

        flc<void()> l(lambda);

        auto ci = callback_intf();
        flc<void()> b(std::bind(&callback_intf::method, ci));
    }

    if (true)
    {
        // Functors:
        callback<void()> cbf;
        callback<void(int, int, bool)> cbp;

        // Classes:
        callback<callback_intf> cbi;

        // Lambda
        {
            auto [cb, cg] = make_callback<void()>(lambda);
            cb.invoke();
        }

        // Plain function
        {
            auto [cb, cg] = make_callback<void()>(function);
            cb.invoke();
        }

        // std::bind functor
        {
            callback_intf ci;
            auto [cb, cg] = make_callback<void()>(std::bind(&callback_intf::method, &ci));
            cb.invoke();
        }

        // std::function functor
        {
            auto f = std::function(function_slow);

            auto [cb, cg] = make_callback(f);
            cb.invoke();

            auto cb_move = std::move(cb);
        }

        // interface callback
        {
            callback_intf ci;

            auto [cb, cg] = make_callback(ci);
            cb.invoke(&callback_intf::method);
        }
    }

    /*
    // Simple callback_store test:
    if (false)
    {
        callback_intf cia;
        callback_intf cib;

        callback_store<callback_intf> cs;
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
        std::size_t count = 100;

        std::vector<callback_intf> ci;
        callback_store<callback_intf> cs;

        for (std::size_t n = 0; n < count; ++n)
        {
            auto it = ci.emplace(ci.end());
            cs.subscribe(*it);
        }
    }

    // Function callback test:
    if (false)
    {
        callback<void()> c;
        callback_guard<void()> g;

        std::tie(c, g) = make_callback(function);

        c.invoke();
    }

    // Function callback_store test:
    if (false)
    {
        callback_store<void()> s;
        auto g = s.subscribe(function);
        auto g_slow = s.subscribe(function_slow);

        s.invoke();
    }

    // callback_listener test:
    if (false)
    {
        callback_intf ci;
        callback_listener<callback_abstract_intf, callback_intf> cl;
        cl.bind(&callback_abstract_intf::method, &callback_intf::method, ci);
        cl.invoke(&callback_abstract_intf::method);

        auto cbpack = make_callback<callback_intf>(cl);
        auto cb = std::move(std::get<0>(cbpack));
        auto cg = std::move(std::get<1>(cbpack));

        //cb.invoke(&callback_intf::method);
    }
    */

    return 0;
}
