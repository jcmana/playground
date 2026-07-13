#pragma once

#include <string_view>

class greeter_intf
{
public:
    virtual ~greeter_intf() = default;

    virtual void greet(std::string_view name) = 0;
};
