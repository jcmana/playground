#pragma once

#include <iostream>

#include "logger_intf.h"

class logger : public logger_intf
{
public:
    virtual void info(std::string_view message) override
    {
        std::cout << "I: " << message << std::endl;
    }
    virtual void warning(std::string_view message) override
    {
        std::cout << "W: " << message << std::endl;
    }
    virtual void error(std::string_view message) override
    {
        std::cout << "E: " << message << std::endl;
    }
};