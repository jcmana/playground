#pragma once

#include "../module_logger/logger_intf.h"

#include "greeter_intf.h"

class greeter : public greeter_intf
{
public:
    greeter(logger_intf * logger_ptr) :
        m_logger_ptr(logger_ptr)
    {
    }

    virtual void greet(std::string_view name) override
    {
        m_logger_ptr->info(std::string("Hello, ") + std::string(name) + "!");
    }

private:
    logger_intf * m_logger_ptr = nullptr;
};
