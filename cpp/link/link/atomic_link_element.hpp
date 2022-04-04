#pragma once

#include <mutex>
#include <tuple>
#include <utility>

class atomic_link_element
{
public:
	atomic_link_element() noexcept;
    atomic_link_element(const atomic_link_element & other) noexcept = delete;
    atomic_link_element(atomic_link_element && other) noexcept;

    ~atomic_link_element();

    atomic_link_element & operator  =(const atomic_link_element & other) noexcept = delete;
    atomic_link_element & operator  =(atomic_link_element && other) noexcept;

    bool linked() const noexcept;

    /// \brief      Locks the linked pair if `linked()` or this element.
    void lock();
    void unlock();

public:
    friend std::tuple<atomic_link_element, atomic_link_element> make_atomic_link();
    friend void swap(atomic_link_element & lhs, atomic_link_element & rhs);

private:
    std::mutex m_mutex;
	atomic_link_element * m_element_ptr;
};

#pragma region link_element implementation:

atomic_link_element::atomic_link_element() noexcept :
	m_element_ptr(nullptr)
{
}

atomic_link_element::atomic_link_element(atomic_link_element && other) noexcept :
    atomic_link_element()
{
    swap(*this, other);
}

atomic_link_element::~atomic_link_element() noexcept
{
    // Critical section:
    {
        std::unique_lock l(*this);
        if (linked())
        {
            m_element_ptr->m_element_ptr = nullptr;
        }
    }

    m_element_ptr = nullptr;
}

atomic_link_element & atomic_link_element::operator  =(atomic_link_element && other) noexcept
{
    atomic_link_element empty;
    
    using std::swap;
    swap(*this, other);
    swap(other, empty);

    return (*this);
}

bool atomic_link_element::linked() const noexcept
{
	return m_element_ptr != nullptr;
}

void atomic_link_element::lock()
{
    while (true)
    {
        m_mutex.lock();

        if (m_element_ptr == nullptr)
        {
            // Link is already broken, we have only local mutex now
            return;
        }

        if (m_element_ptr->m_mutex.try_lock())
        {
            // Both mutexes are acquired, we have a lock
            return;
        }

        m_mutex.unlock();

        // Somebody works on this pair and locked faster, let him finish the work
        std::this_thread::yield();
    }
}

void atomic_link_element::unlock()
{
    if (m_element_ptr != nullptr)
    {
        m_element_ptr->m_mutex.unlock();
    }

    m_mutex.unlock();
}

std::tuple<atomic_link_element, atomic_link_element> make_atomic_link()
{
    atomic_link_element a;
    atomic_link_element b;

    a.m_element_ptr = &b;
    b.m_element_ptr = &a;

    return {std::move(a), std::move(b)};
}

void swap(atomic_link_element & lhs, atomic_link_element & rhs)
{
    // Case #1:
    //  - pair a: l----r
    //  - pair b: l----r
    //  - swapping between pairs
    //
    // Case #2:
    //  - pair a: l----r
    //  - swapping in one pair
    //
    // Case #3:
    //  - pair a: l----r
    //  - pair b: l-   x
    //  - swapping linked with unlinked
    //
    // Case #4:
    //  - pair a: l-   x
    //  - pair b: l-   x
    //  - swapping unlinked with unlinked

    std::unique_lock ll(lhs);

    if (lhs.m_element_ptr == &rhs && rhs.m_element_ptr == &lhs)
    {
        return;
    }

    std::unique_lock lr(rhs);

    using std::swap;
    swap(lhs.m_element_ptr, rhs.m_element_ptr);

    if (lhs.linked())
    {
        lhs.m_element_ptr->m_element_ptr = &lhs;
    }

    if (rhs.linked())
    {
        rhs.m_element_ptr->m_element_ptr = &rhs;
    }
}

#pragma endregion

