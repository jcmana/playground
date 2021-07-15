#pragma once

#include <functional>

struct Model
{
    int value = 0;

    std::function<void(int)> callback;
    std::function<void()> event;
};