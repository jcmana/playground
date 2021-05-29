#pragma once

#include <vector>
#include <functional>

#include "slot.hpp"

template<typename ... A>
struct signal
{
    void connect(slot<A ...> & st)
    {
        m_slots.emplace_back(st);
    }

    void send(A ... arguments)
    {
        for (auto && slot : m_slots)
        {
            slot.get().receive(std::forward<A>(arguments) ...);
        }
    }

    std::vector<std::reference_wrapper<slot<A ...>>> m_slots;
};
