#pragma once

#include <tuple>
#include <utility>

#include "../../link/link/atomic_link_element.hpp"

template<typename T>
class atomic_callback_guard;

template<typename T>
class atomic_callback : private atomic_link_element
{
public:
    friend class atomic_callback_guard<T>;

public:
    atomic_callback() :
        atomic_link_element(),
        m_interface_ptr(nullptr)
    {
    }

    atomic_callback(T & interface_ref, atomic_link_element && link_element) :
        atomic_link_element(std::move(link_element)),
        m_interface_ptr(&interface_ref)
    {
    }

    template<typename F, typename ... Args>
    void invoke(F method_ptr, Args && ... args) const
    {
        auto lock = atomic_link_element::lock();

        if (atomic_link_element::linked())
        {
            (m_interface_ptr->*method_ptr)(std::forward<Args>(args) ...);
        }
    }

    template<typename T>
    friend std::tuple<atomic_callback<T>, atomic_callback_guard<T>> make_atomic_callback(T & interface_ref);

private:
    T * m_interface_ptr;
};

template<typename T>
std::tuple<atomic_callback<T>, atomic_callback_guard<T>> make_atomic_callback(T & interface_ref)
{
    auto link = make_atomic_link();

    atomic_callback<T> callback(interface_ref, std::move(std::get<0>(link)));
    atomic_callback_guard<T> callback_guard(std::move(std::get<1>(link)));

    return std::make_tuple(std::move(callback), std::move(callback_guard));
}