#pragma once

#include <memory>

#include "../framework/service_intf.h"

#include "logger.h"

class service_logger : public service_intf
{
public:
    static constexpr auto id = "std-out";

public:
    service_logger()
    {
        m_up = std::make_unique<logger>();
    }

public:
    virtual void * facade() const override
    {
        return m_up.get();
    }

private:
    std::unique_ptr<logger> m_up;
};