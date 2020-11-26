#include <iostream>

#include "observable.hpp"

void main()
{
    observable o;

    // Implicit conversion to `int`
    std::cout << o << std::endl;
}