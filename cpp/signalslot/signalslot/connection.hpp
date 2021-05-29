#pragma once

#include "../../link/link/link_element.hpp"

#include "signal.hpp"
#include "slot.hpp"

struct connection : link_element
{
};

template<typename ... A>
void connect(signal<A ...> & sl, slot<A ...> & st)
{
    sl.connect(st);
}