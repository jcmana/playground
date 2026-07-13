#pragma once

#include <format>

#include "../framework/module_intf.h"
#include "../framework/service_store_intf.h"

#include "service_logger.h"

class module_logger : public module_intf
{
public:
    static constexpr auto id = "logger";

    std::string logger_service_id() const
    {
        return std::format("{}.{}", id, service_logger::id);
    }

public:
    virtual void load(service_store_intf & ref) override
    {
        m_up = std::make_unique<service_logger>();
        ref.reset(logger_service_id().c_str(), m_up.get());
    }

    virtual void unload(service_store_intf & ref) override
    {
        ref.reset(logger_service_id().c_str());
        m_up.reset();
    }

public:
    std::unique_ptr<service_logger> m_up;
};