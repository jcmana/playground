#pragma once

#include <memory>
#include <map>
#include <string>

#include "service_store_intf.h"

class service_store : public service_store_intf
{
public:
    // service_store_intf implementation:
    virtual void reset(const char * id) override
    {
    }

    virtual void reset(const char * id, service_intf * service_ptr) override
    {
        m_services.emplace(id, service_ptr);
    }

    virtual service_intf * get(const char * id) const override
    {
        // JMTODO: blocking mechanics

        return m_services.at(id).get();
    }

    virtual service_intf * try_get(const char * id) const override
    {
        // JMTODO: optional service access
        return nullptr;
    }

private:
    std::map<std::string, std::unique_ptr<service_intf>> m_services;
};