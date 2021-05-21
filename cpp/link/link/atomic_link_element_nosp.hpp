#pragma once

#include <mutex>
#include <tuple>
#include <utility>

/// \brief			Element of linked pair, unlinked on destruction of either one.
///
/// Creates unique link between pair of elements, which is kept through moves and
/// broken when either of them is destroyed. 
class atomic_link_element_nosp
{
public:
	/// \brief			Default constructor, creates unlinked element.
	atomic_link_element_nosp() noexcept;

	/// \brief			Move constructor, overtakes the link from `other`.
	/// 
	/// Overtakes the link from `other` element, if there was such.
    atomic_link_element_nosp(atomic_link_element_nosp && other) noexcept;

	/// \brief			Destructor, destroys the link between elements, if any.
	///
	/// Destroys `this` element and unlinks the linked element, if there was such.
    ~atomic_link_element_nosp();

	/// \brief			Move assignment, overtakes the link from `other`.
	///
	/// Overtakes the link from `other` element, if there was such.
    atomic_link_element_nosp & operator  =(atomic_link_element_nosp && other) noexcept;

	/// \brief			Checks if element is linked.
	/// \returns		`true` if both elements are linked together; `false` otherwise.
    bool linked() const noexcept;

    /// \brief          Creates pair of linked elements.
    friend std::tuple<atomic_link_element_nosp, atomic_link_element_nosp> make_atomic_link_nosp();

    /// \brief          Swaps the linked elements between `lhs` and `rhs`.
    friend void swap(atomic_link_element_nosp & lhs, atomic_link_element_nosp & rhs) noexcept;

private:
    std::mutex * m_mutex_ptr;
	atomic_link_element_nosp * m_element_ptr;
};

#pragma region link_element implementation:

atomic_link_element_nosp::atomic_link_element_nosp() noexcept :
	m_element_ptr(nullptr)
{
    m_mutex_ptr = new std::mutex();
}

atomic_link_element_nosp::atomic_link_element_nosp(atomic_link_element_nosp && other) noexcept :
    atomic_link_element_nosp()
{
    swap(*this, other);
}

atomic_link_element_nosp::~atomic_link_element_nosp()
{
    // Critical section:
    {
        std::unique_lock<std::mutex> lock(*m_mutex_ptr);

        if (linked())
        {
            m_element_ptr->m_element_ptr = nullptr;
            m_element_ptr = nullptr;
        
            return;
        }
    }

    delete m_mutex_ptr;
}

atomic_link_element_nosp & atomic_link_element_nosp::operator  =(atomic_link_element_nosp && other) noexcept
{
    auto empty = atomic_link_element_nosp();

    swap(*this, empty);
    swap(*this, other);

	return (*this);
}

bool atomic_link_element_nosp::linked() const noexcept
{
	return m_element_ptr != nullptr;
}

std::tuple<atomic_link_element_nosp, atomic_link_element_nosp> make_atomic_link_nosp()
{
    atomic_link_element_nosp a;
    atomic_link_element_nosp b;

    a.m_element_ptr = &b;
    b.m_element_ptr = &a;

    delete b.m_mutex_ptr;
    b.m_mutex_ptr = a.m_mutex_ptr;

    return std::make_tuple(std::move(a), std::move(b));
}

void swap(atomic_link_element_nosp & lhs, atomic_link_element_nosp & rhs) noexcept
{
    if (lhs.m_element_ptr == &rhs && rhs.m_element_ptr == &lhs)
    {
        return;
    }

    // Critical section:
    {
        std::unique_lock<std::mutex> lock_lhs(*lhs.m_mutex_ptr);
        std::unique_lock<std::mutex> lock_rhs(*rhs.m_mutex_ptr);

        std::swap(lhs.m_mutex_ptr, rhs.m_mutex_ptr);
        std::swap(lhs.m_element_ptr, rhs.m_element_ptr);

        if (lhs.linked())
        {
            lhs.m_element_ptr->m_element_ptr = &lhs;
        }

        if (rhs.linked())
        {
            rhs.m_element_ptr->m_element_ptr = &rhs;
        }
    }
}

#pragma endregion
