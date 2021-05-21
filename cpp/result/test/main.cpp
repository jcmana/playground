#include <iostream>

#include "../../error/error/error.h"
#include "../result/result.hpp"

static constexpr error OK = {};

int main()
{
    result<int, error> r = OK;

    if (r)
    {
        std::cout << r.value() << std::endl;
    }
    else
    {
        std::cout << "error" << std::endl;
    }
}