#include <iostream>

#include "../../error/error/error.h"
#include "../result/result.hpp"

static constexpr error OK = {};

int main()
{
    result<int, error> r;

    if (!r)
    {
        std::cout << "error" << std::endl;
        return 0;
    }

    std::cout << r.value() << std::endl;
}