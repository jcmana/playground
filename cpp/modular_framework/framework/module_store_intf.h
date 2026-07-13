#pragma once

#include "module_intf.h"

class module_store_intf
{
public:
    virtual ~module_store_intf() = default;

    virtual void add(module_intf * ptr) = 0;
    virtual void remove(module_intf * ptr) = 0;
};