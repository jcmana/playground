#pragma once

#include <utility>

/// \brief			Element of linked pair, broken on destruction of either one.
///
/// Creates unique link between pair of elements, which is kept through moves and
/// broken when either of them is destroyed. 
///
/// Link can be made only between pair of unlinked elements. Linking to an already
/// linked element results in no action = original link is kept, current element is
/// left unlinked.
class link_element
{
public:
	/// \brief			Default constructor, creates unlinked element.
	inline link_element() noexcept;

	/// \brief			Constructor, creates element linked to `element_ptr`.
	///
	/// Ensures that both `this` and `element_ptr` are linked together, if they were
	/// both unlinked before the call.
	inline explicit link_element(link_element * element_ptr) noexcept;

	/// \brief			Copy constructor, deleted.
	link_element(const link_element & other) = delete;

	/// \brief			Move constructor, overtakes the link from `other`.
	/// 
	/// Takes the link from `other` element, if there was such.
	inline link_element(link_element && other) noexcept;

	/// \brief			Destructor, destroys the link between elements, if any.
	///
	/// Destroys `this` element and unlinks the linked element, if there was such.
	inline ~link_element();

	/// \brief			Copy assignment, deleted.
	inline link_element & operator  =(const link_element & other) = delete;

	/// \brief			Move assignment, overtakes the link from `other`.
	///
	/// Takes the link from `other` element, if there was such and if `this` element
	/// was unlinked.
	inline link_element & operator  =(link_element && other) noexcept;

	/// \brief			Checks if element is linked.
	/// \returns		`true` if both elements are linked together; `false` otherwise.
	inline bool is_linked() const noexcept;

private:
	link_element * m_element_ptr;
};

link_element::link_element() noexcept :
	m_element_ptr(nullptr)
{
}

link_element::link_element(link_element * element_ptr) noexcept :
	m_element_ptr(element_ptr)
{
	if (m_element_ptr)
	{
		if (m_element_ptr->m_element_ptr == nullptr)
		{
			m_element_ptr->m_element_ptr = this;
		}
	}
}

link_element::link_element(link_element && other) noexcept :
	m_element_ptr(nullptr)
{
	if (m_element_ptr == nullptr)
	{
		m_element_ptr = other.m_element_ptr;
		other.m_element_ptr = nullptr;

		if (m_element_ptr)
		{
			m_element_ptr->m_element_ptr = this;
		}
	}
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
	if (m_element_ptr == nullptr)
	{
		m_element_ptr = other.m_element_ptr;
		other.m_element_ptr = nullptr;

		if (m_element_ptr)
		{
			m_element_ptr->m_element_ptr = this;
		}
	}

	return (*this);
}

bool link_element::is_linked() const noexcept
{
	return m_element_ptr;
}
