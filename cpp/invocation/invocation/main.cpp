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

void main()
{
    auto l = [](int n)
    {
        std::cout << "lambda[] = " << n << std::endl;
    };

    invocation::lambda il(l);
    il.invoke(6);

    interface i;
    invocation::interface ii(i);
    ii.invoke(&interface::method_parametrized, 7);
}