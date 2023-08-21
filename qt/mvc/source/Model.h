#pragma once

#include <functional>

struct Model
{
    void set(int v)
    {
        value = v;

        if (callback)
        {
            callback(value);
        }

        if (event)
        {
            event();
        }
    }

    int value = 0;

    std::function<void(int)> callback;
    std::function<void()> event;
};