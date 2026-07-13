#pragma once

#include "service_intf.h"

class service_store_intf
{
public:
    virtual ~service_store_intf() = default;

    virtual void reset(const char * id) = 0;
    virtual void reset(const char * id, service_intf * service_ptr) = 0;

    virtual service_intf * get(const char * id) const = 0;
    virtual service_intf * try_get(const char * id) const = 0;

    template<typename T>
    T * get_facade(const char * id)
    {
        return static_cast<T *>(get(id)->facade());
    }

    template<typename T>
    T * try_get_facade(const char * id)
    {
        return static_cast<T *>(try_get(id)->facade());
    }
};
