#pragma once

#include "signal.hpp"
#include "slot.hpp"

template<typename ... A>
void connect(signal<A ...> & signal, slot<A ...> & slot)
{
    auto guard = signal.m_store.subscribe(std::ref(slot));
    slot.m_connection.push_back(std::move(guard));
}