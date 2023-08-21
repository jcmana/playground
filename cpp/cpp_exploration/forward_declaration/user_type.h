#pragma once

#include <iostream>
#include "config.h"

class user_type
{
public:
    user_type()
    {
        std::cout << "user_type::ctor()" << std::endl;
    }

    ~user_type()
    {
        std::cout << "user_type::dtor()" << std::endl;
    }

    inline void method()
    {
        std::cout << "user_type::method()" << std::endl;
    }
};