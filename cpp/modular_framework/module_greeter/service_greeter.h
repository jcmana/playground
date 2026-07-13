#pragma once

#include <memory>

#include "../framework/service_intf.h"
#include "../module_logger/logger_intf.h"

#include "greeter.h"

class service_greeter : public service_intf
{
public:
    static constexpr auto id = "default";

public:
    service_greeter(logger_intf * logger_ptr)
    {
        m_up = std::make_unique<greeter>(logger_ptr);
    }

public:
    virtual void * facade() const override
    {
        return m_up.get();
    }

private:
    std::unique_ptr<greeter> m_up;
};
