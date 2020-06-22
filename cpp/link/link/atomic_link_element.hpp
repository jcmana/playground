#pragma once

#include <mutex>
#include <memory>
#include <tuple>

class atomic_link_element
{
public:
    atomic_link_element() noexcept :
        m_sp_mutex(new std::mutex),
        m_element_ptr(nullptr)
    {
    }

    atomic_link_element(atomic_link_element && other) noexcept :
        atomic_link_element()
    {
        swap(*this, other);
    }

    ~atomic_link_element()
    {
        std::unique_lock<std::mutex> lock(*m_sp_mutex);

        if (m_element_ptr)
        {
            m_element_ptr->m_element_ptr = nullptr;
            m_element_ptr = nullptr;
        }
    }

    atomic_link_element & operator  =(atomic_link_element && other) noexcept
    {
        auto empty = atomic_link_element();

        swap(*this, empty);
        swap(*this, other);

        return (*this);
    }

    bool is_linked() const noexcept
    {
        return m_element_ptr != nullptr;
    }

    std::unique_lock<std::mutex> lock() const
    {
        return std::unique_lock<std::mutex>(*m_sp_mutex);
    }

    friend std::tuple<atomic_link_element, atomic_link_element> make_atomic_link();

    friend void swap(atomic_link_element & lhs, atomic_link_element & rhs);

private:
    mutable std::shared_ptr<std::mutex> m_sp_mutex;

    atomic_link_element * m_element_ptr;
};

std::tuple<atomic_link_element, atomic_link_element> make_atomic_link()
{
    atomic_link_element a;
    atomic_link_element b;

    a.m_sp_mutex = b.m_sp_mutex;

    a.m_element_ptr = &b;
    b.m_element_ptr = &a;

    return std::make_tuple(std::move(a), std::move(b));
}

void swap(atomic_link_element & lhs, atomic_link_element & rhs)
{
    std::unique_lock<std::mutex> lhs_lock(*lhs.m_sp_mutex);

    if (lhs.m_sp_mutex == rhs.m_sp_mutex)
    {
        // JMTODO: condition race here

        return;
    }

    std::unique_lock<std::mutex> rhs_lock(*rhs.m_sp_mutex);

    using std::swap;

    swap(lhs.m_element_ptr, rhs.m_element_ptr);

    if (lhs.is_linked())
    {
        lhs.m_element_ptr->m_element_ptr = &lhs;
    }

    if (rhs.is_linked())
    {
        rhs.m_element_ptr->m_element_ptr = &rhs;
    }

    swap(lhs.m_sp_mutex, rhs.m_sp_mutex);
}
