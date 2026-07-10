#pragma once

#include "service_store_intf.h"

class module_intf
{
public:
    virtual ~module_intf() = default;
    virtual void load(service_store_intf & ref) = 0;
};