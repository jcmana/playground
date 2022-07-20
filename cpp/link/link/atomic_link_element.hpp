#pragma once

#include <mutex>
#include <tuple>
#include <utility>

template<typename M = std::mutex>
class atomic_link_element
{
public:
    using mutex_type = M;

public:
	atomic_link_element() noexcept;
    atomic_link_element(const atomic_link_element & other) noexcept = delete;
    atomic_link_element(atomic_link_element && other) noexcept;

    ~atomic_link_element();

    atomic_link_element & operator  =(const atomic_link_element & other) noexcept = delete;
    atomic_link_element & operator  =(atomic_link_element && other) noexcept;

    bool linked() const noexcept;

    void lock() const;
    bool try_lock() const;
    void unlock() const;

public:
    template<typename FM>
    friend std::tuple<atomic_link_element<FM>, atomic_link_element<FM>> make_atomic_link();

    template<typename FM>
    friend void swap(atomic_link_element<FM> & lhs, atomic_link_element<FM> & rhs);

private:
    mutable mutex_type m_mutex;
	atomic_link_element * m_element_ptr;
};

#pragma region link_element implementation:

template<typename M>
atomic_link_element<M>::atomic_link_element() noexcept :
	m_element_ptr(nullptr)
{
}

template<typename M>
atomic_link_element<M>::atomic_link_element(atomic_link_element && other) noexcept :
    atomic_link_element()
{
    swap(*this, other);
}

template<typename M>
atomic_link_element<M>::~atomic_link_element()
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

template<typename M>
atomic_link_element<M> & 
atomic_link_element<M>::operator  =(atomic_link_element && other) noexcept
{
    atomic_link_element empty;
    
    using std::swap;
    swap(*this, other);
    swap(other, empty);

    return (*this);
}

template<typename M>
bool 
atomic_link_element<M>::linked() const noexcept
{
	return m_element_ptr != nullptr;
}

template<typename M>
void 
atomic_link_element<M>::lock() const
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

template<typename M>
bool 
atomic_link_element<M>:: try_lock() const
{
    if (m_mutex.try_lock())
    {
        if (m_element_ptr == nullptr)
        {
            // Link is already broken, we have only local mutex now
            return true;
        }

        if (m_element_ptr->m_mutex.try_lock())
        {
            // Both mutexes are acquired, we have a lock
            return true;
        }

        // THIS IS RACE BETWEEN THE TWO TRY_LOCKS:
        // 1. both outer try_lock()s succeeds
        // 2. both inner try_lock()s fails
        // == one of the concurrent calls has to return true, one false
    }

    return false;
}

template<typename M>
void
atomic_link_element<M>::unlock() const
{
    if (m_element_ptr != nullptr)
    {
        m_element_ptr->m_mutex.unlock();
    }

    m_mutex.unlock();
}

template<typename M = std::mutex>
std::tuple<atomic_link_element<M>, atomic_link_element<M>> 
make_atomic_link()
{
    atomic_link_element<M> a;
    atomic_link_element<M> b;

    a.m_element_ptr = &b;
    b.m_element_ptr = &a;

    return {std::move(a), std::move(b)};
}

template<typename M>
void 
swap(atomic_link_element<M> & lhs, atomic_link_element<M> & rhs)
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

