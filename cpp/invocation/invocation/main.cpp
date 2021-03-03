#include <iostream>
#include <functional>
#include <utility>

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

    bool method_retval()
    {
        std::cout << "interface::method_retval()" << std::endl;
        return true;
    }
};

void function()
{
    std::cout << "function()" << std::endl;
}

struct functor
{
    void operator ()()
    {
        std::cout << "functor::operator()" << std::endl;
    }

    void operator ()(int n)
    {
        std::cout << "functor::operator(int)" << std::endl;
    }

    bool operator ()(int m, int n)
    {
        std::cout << "interface::operator(int, int)" << std::endl;
        return false;
    }
};

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

        invocation::functor i(l);
        i.invoke(7);
    }

    // Interface invocation:
    if (true)
    {
        interface intf;
        invocation::interface i(intf);
        i.invoke(&interface::method_parametrized, 7);
        auto r = i.invoke(&interface::method_retval);
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
        invocation::functor i(b);
        i.invoke(7);
    }

    // Custom functor invocation:
    if (false)
    {
        functor f;
        invocation::functor i(f);
        i.invoke();
        i.invoke(7);
        auto r = i.invoke(7, 3);
    }
}