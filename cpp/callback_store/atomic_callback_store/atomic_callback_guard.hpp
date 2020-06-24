#pragma once

#include <utility>

#include "../../link/link/atomic_link_element.hpp"

template<typename T>
class atomic_callback;

template<typename T>
class atomic_callback_guard : private atomic_link_element
{
public:
    friend class atomic_callback<T>;

public:
    atomic_callback_guard()
    {
    }

    atomic_callback_guard(atomic_link_element && link_element) :
        atomic_link_element(std::move(link_element))
    {
    }
};