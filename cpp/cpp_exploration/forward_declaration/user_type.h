#pragma once

#include <iostream>
#include "config.h"

class user_type
{
public:
    inline void method()
    {
        std::cout << "user_type::method()" << std::endl;
    }
};