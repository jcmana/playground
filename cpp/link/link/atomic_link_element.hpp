#pragma once

#include <mutex>
#include <memory>
#include <tuple>

/// \brief      Thread-safe `link_element` implementation.
class atomic_link_element
{
public:
    inline atomic_link_element() noexcept;

    inline atomic_link_element(atomic_link_element && other) noexcept;

    inline ~atomic_link_element();

    inline atomic_link_element & operator  =(atomic_link_element && other) noexcept;

    inline bool linked() const noexcept;

    /// \brief      Locks the link for the duration of returned `std::unique_lock`.
    inline std::unique_lock<std::mutex> lock();

    friend inline std::tuple<atomic_link_element, atomic_link_element> make_atomic_link();

    friend inline void swap(atomic_link_element & lhs, atomic_link_element & rhs);

private:
    std::shared_ptr<std::mutex> m_sp_mutex;
};

atomic_link_element::atomic_link_element() noexcept :
    m_sp_mutex(new std::mutex)
{
}

atomic_link_element::atomic_link_element(atomic_link_element && other) noexcept :
    atomic_link_element()
{
    swap(*this, other);
}

atomic_link_element::~atomic_link_element()
{
    std::unique_lock<std::mutex> lock(*m_sp_mutex);
}

atomic_link_element & atomic_link_element::operator  =(atomic_link_element && other) noexcept
{
    auto empty = atomic_link_element();

    swap(*this, empty);
    swap(*this, other);

    return (*this);
}

bool atomic_link_element::linked() const noexcept
{
    return m_sp_mutex.use_count() == 2;
}

std::unique_lock<std::mutex> atomic_link_element::lock()
{
    return std::unique_lock<std::mutex>(*m_sp_mutex);
}

std::tuple<atomic_link_element, atomic_link_element> make_atomic_link()
{
    atomic_link_element a;
    atomic_link_element b;

    a.m_sp_mutex = b.m_sp_mutex;

    return std::make_tuple(std::move(a), std::move(b));
}

void swap(atomic_link_element & lhs, atomic_link_element & rhs)
{
    if (lhs.m_sp_mutex == rhs.m_sp_mutex)
    {
        return;
    }

    std::unique_lock<std::mutex> lhs_lock(*lhs.m_sp_mutex);
    std::unique_lock<std::mutex> rhs_lock(*rhs.m_sp_mutex);

    std::swap(lhs.m_sp_mutex, rhs.m_sp_mutex);
}
