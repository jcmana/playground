#pragma once

class service_intf
{
public:
    virtual ~service_intf() = default;
    virtual void * facade() const = 0;
};