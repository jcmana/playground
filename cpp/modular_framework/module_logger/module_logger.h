#pragma once

#include "../framework/module_intf.h"
#include "../framework/service_store_intf.h"

class module_logger : public module_intf
{
public:
    virtual void load(service_store_intf & ref) override
    {

    }
};