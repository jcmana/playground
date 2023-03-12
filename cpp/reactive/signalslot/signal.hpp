#pragma once

#include <functional>

#include "../../callback_store/atomic_callback_store/atomic_callback_store.hpp"

template<typename ... A>
class slot;

template<typename ... A>
class signal
{
public:
    using signature = void(A ...);

public:
    void operator ()(A ... args)
    {
        m_store.invoke(std::forward<A>(args) ...);
    }

    template<typename ... A>
    friend void connect(signal<A ...> & signal, slot<A ...> & slot);

private:
    atomic_callback_store<std::function<void(A ...)>> m_store;
};
