#pragma once

#include "service_intf.h"

class service_store_intf
{
public:
    virtual ~service_store_intf() = default;

    virtual void set(const char * id, service_intf * service_ptr) = 0;
    virtual service_intf * get(const char * id) const = 0;
};
