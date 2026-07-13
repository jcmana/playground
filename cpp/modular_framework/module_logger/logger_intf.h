#pragma once

#include <string_view>

class logger_intf
{
public:
    virtual ~logger_intf() = default;

    virtual void info(std::string_view message) = 0;
    virtual void warning(std::string_view message) = 0;
    virtual void error(std::string_view message) = 0;
};