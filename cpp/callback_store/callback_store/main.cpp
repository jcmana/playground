#include <iostream>

#include <thread>
#include <chrono>

#include "callback_store.hpp"
#include "callback_listener.hpp"

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
    std::cout << "function(): " << std::endl;
}

void function_slow()
{
    std::cout << "function_slow()" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main()
{
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
    if (true)
    {
        callback_intf ci;
        callback_listener<callback_abstract_intf, callback_intf> cl;
        cl.bind(&callback_abstract_intf::method, &callback_intf::method, ci);
        cl.invoke(&callback_abstract_intf::method);

        /*
        auto cbpack = make_callback<callback_intf>(cl);
        auto cb = std::move(std::get<0>(cbpack));
        auto cg = std::move(std::get<1>(cbpack));
        */

        //cb.invoke(&callback_intf::method);
    }

    return 0;
}
