#pragma once

#include <functional>
#include <vector>

#include "../../callback_store/atomic_callback_store/atomic_callback_guard.hpp"

template<typename ... A>
class signal;

template<typename ... A>
class slot
{
public:
    using signature = void(A ...);

public:
    slot()
    {
    }

    template<typename F>
    slot(F && functor) :
        m_functor(functor)
    {
    }

    void operator ()(A ... args)
    {
        if (m_functor)
        {
            m_functor(std::forward<A>(args) ...);
        }
    }

    template<typename ... A>
    friend void connect(signal<A ...> & signal, slot<A ...> & slot);

private:
    std::function<void(A ...)> m_functor;
    std::vector<atomic_callback_guard<std::function<signature>>> m_connection;
};