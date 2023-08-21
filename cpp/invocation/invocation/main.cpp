#include <iostream>
#include <functional>
#include <utility>

#include <string>
#include <algorithm>

#include "interface.hpp"

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

void main()
{
    interface intf;
    invocation::interface i(intf);
    i(&interface::method);
    i(&interface::method_parametrized, 7);
    auto r = i(&interface::method_retval);
}