#include <iostream>

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
    if (true)
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
}