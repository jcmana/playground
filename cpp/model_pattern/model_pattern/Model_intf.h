#pragma once

#include "Observable_intf.h"
#include "Waitable_intf.h"

class IModel :
    public IObservable,
    public IWaitable
{
public:
    virtual ~IModel() = default;
};