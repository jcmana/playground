#pragma once

#include <tuple>
#include <utility>

/// \brief			Element of linked pair, unlinked on destruction of either one.
///
/// Creates unique link between pair of elements, which is kept through moves and
/// broken when either of them is destroyed. 
class link_element
{
public:
	/// \brief			Default constructor, creates unlinked element.
	inline link_element() noexcept;

	/// \brief			Move constructor, overtakes the link from `other`.
	/// 
	/// Overtakes the link from `other` element, if there was such.
    inline link_element(link_element && other) noexcept;

	/// \brief			Destructor, destroys the link between elements, if any.
	///
	/// Destroys `this` element and unlinks the linked element, if there was such.
    inline ~link_element();

	/// \brief			Move assignment, overtakes the link from `other`.
	///
	/// Overtakes the link from `other` element, if there was such.
    inline link_element & operator  =(link_element && other) noexcept;

	/// \brief			Checks if element is linked.
	/// \returns		`true` if both elements are linked together; `false` otherwise.
    inline bool linked() const noexcept;

    /// \brief          Creates pair of linked elements.
    inline friend std::tuple<link_element, link_element> make_link();

    /// \brief          Swaps the linked elements between `lhs` and `rhs`.
    inline friend void swap(link_element & lhs, link_element & rhs) noexcept;

private:
	link_element * m_element_ptr;
};

#pragma region link_element implementation:

link_element::link_element() noexcept :
	m_element_ptr(nullptr)
{
}

link_element::link_element(link_element && other) noexcept :
    link_element()
{
    swap(*this, other);
}

link_element::~link_element()
{
    if (m_element_ptr)
    {
        m_element_ptr->m_element_ptr = nullptr;
        m_element_ptr = nullptr;
    }
}

link_element & link_element::operator  =(link_element && other) noexcept
{
    auto empty = link_element();

    swap(*this, empty);
    swap(*this, other);

	return (*this);
}

bool link_element::linked() const noexcept
{
	return m_element_ptr != nullptr;
}

std::tuple<link_element, link_element> make_link()
{
    link_element a;
    link_element b;

    a.m_element_ptr = &b;
    b.m_element_ptr = &a;

    return std::make_tuple(std::move(a), std::move(b));
}

void swap(link_element & lhs, link_element & rhs) noexcept
{
    if (lhs.m_element_ptr == &rhs && rhs.m_element_ptr == &lhs)
    {
        return;
    }

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

#pragma endregion
