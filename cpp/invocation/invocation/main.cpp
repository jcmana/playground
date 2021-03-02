#include <iostream>
#include <functional>

#include "invocation.hpp"

struct interface
{
    void method()
    {
        std::cout << "interface::method()" << std::endl;
    }

    void method_parametrized(int n)
    {
        std::cout << "interface::method_parametrized() = " << n << std::endl;
    }
};

void function()
{
    std::cout << "function()" << std::endl;
}

void main()
{
    // Lambda decay to function:
    if (false)
    {
        using function_t = void(*)(int);

        function_t l = nullptr;

        {
            l = [](int n)
            {
                std::cout << "lambda[] = " << n << std::endl;
            };

            // Decays automatically if there is no capture
        }

        l(12);
    }

    // Function:
    if (false)
    {
        invocation::function i(function);
        i.invoke();
    }

    // Lambda invocation:
    if (false)
    {
        auto l = [](int n)
        {
            std::cout << "lambda[] = " << n << std::endl;
        };

        invocation::lambda i(l);
        i.invoke(6);
    }

    // Interface invocation:
    if (false)
    {
        interface intf;
        invocation::interface i(intf);
        i.invoke(&interface::method_parametrized, 7);
    }

    // std::function invocation:
    if (false)
    {
        std::function f(function);
        invocation::functor i(f);
        i.invoke();
    }

    // std::bind invocation:
    if (false)
    {
        interface intf;
        auto b = std::bind(&interface::method_parametrized, &intf, std::placeholders::_1);
        //b(7);
        //using F = decltype(b.operator()());
        //using T = decltype(b);
        //void(T:: * ptr)(int) const = &T::operator();
        //(b.*ptr)(7);
        //invocation::functor i(b);
        //i.invoke();

        // CAN'T MAKE THIS WORK
        // std::bind SEEMS LIKE TEMPLATE BULLSHIT
        // USE invocation::interface INSTEAD
    }
}