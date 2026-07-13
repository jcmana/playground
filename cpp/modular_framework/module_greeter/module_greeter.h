#pragma once

#include <thread>
#include <memory>
#include <format>

#include "../framework/module_intf.h"
#include "../framework/service_store_intf.h"
#include "../module_logger/logger_intf.h"

#include "service_greeter.h"

class module_greeter : public module_intf
{
public:
    static constexpr auto id = "greeter";

    std::string greeter_service_id() const
    {
        return std::format("{}.{}", id, service_greeter::id);
    }

public:
    virtual void load(service_store_intf & ref) override
    {
        auto * logger_service_ptr = ref.get("logger.std-out");
        auto * logger_ptr = static_cast<logger_intf *>(logger_service_ptr->facade());

        m_up = std::make_unique<service_greeter>(logger_ptr);
        ref.reset(greeter_service_id().c_str(), m_up.get());

        auto * greeter_ptr = ref.get_facade<greeter_intf>("greeter.default");
        greeter_ptr->greet("greeter start");
    }

    virtual void unload(service_store_intf & ref) override
    {
        ref.reset(greeter_service_id().c_str());
        m_up.reset();
    }

public:
    std::unique_ptr<service_greeter> m_up;
};
