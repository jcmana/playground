#pragma once

#include <mutex>
#include <memory>
#include <tuple>

/// \brief      Thread-safe `link_element` implementation.
class atomic_link_element
{
public:
    /// \copydoc    link_element::link_element() noexcept
    inline atomic_link_element() noexcept;

    /// \copydoc    link_element::link_element(atomic_link_element && other) noexcept
    inline atomic_link_element(atomic_link_element && other) noexcept;

    inline ~atomic_link_element();

    inline atomic_link_element & operator  =(atomic_link_element && other) noexcept;

    /// \copydoc    link_element::linked()
    inline bool linked() const noexcept;

public:
    // BasicLockable concept implementation:

    /// \brief      Locks the mutex, blocks if the mutex is not available.
    inline void lock() const;

    /// \brief      Unlocks the mutex.
    inline void unlock() const;

public:
    /// \copydoc    make_link()
    friend inline std::tuple<atomic_link_element, atomic_link_element> make_atomic_link();

    /// \copydoc    swap(link_element &, link_element &)
    friend inline void swap(atomic_link_element & lhs, atomic_link_element & rhs);

private:
    std::shared_ptr<std::mutex> m_sp_mutex;
};

#pragma region atomic_link_element implementation:

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
    // Synchronize after all currently running operations are done and clear
    std::unique_lock<std::mutex> lock(*m_sp_mutex);
}

atomic_link_element & atomic_link_element::operator  =(atomic_link_element && other) noexcept
{
    // Swap other for a default element, which means:
    // 1. calling a destructor, unlinking potentionally linked element
    // 2. making this element default unlinked
    auto empty = atomic_link_element();
    swap(*this, empty);

    // Swap this now default and therefore unlinked element for `other`
    swap(*this, other);

    return (*this);
}

bool atomic_link_element::linked() const noexcept
{
    // Elements are considered linked iff 2 are synchronized on a common mutex
    return m_sp_mutex.use_count() == 2;
}

void atomic_link_element::lock() const
{
    m_sp_mutex->lock();
}

void atomic_link_element::unlock() const
{
    m_sp_mutex->unlock();
}

std::tuple<atomic_link_element, atomic_link_element> make_atomic_link()
{
    // Create two default unlinked elements
    atomic_link_element a;
    atomic_link_element b;

    // Link them together by sharing common mutex in shared_ptr
    // 1. default mutex from 'b' is destroyed
    // 2. default mutex from 'a' is assigned to 'b'
    // == both share and synchronize on common mutex
    a.m_sp_mutex = b.m_sp_mutex;

    return std::make_tuple(std::move(a), std::move(b));
}

void swap(atomic_link_element & lhs, atomic_link_element & rhs)
{
    // Avoid recursive locking if we are swapping two linked elements by no-op
    if (lhs.m_sp_mutex == rhs.m_sp_mutex)
    {
        return;
    }

    std::unique_lock<std::mutex> lhs_lock(*lhs.m_sp_mutex);
    std::unique_lock<std::mutex> rhs_lock(*rhs.m_sp_mutex);

    std::swap(lhs.m_sp_mutex, rhs.m_sp_mutex);
}

#pragma endregion
