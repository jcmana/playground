#pragma once

#include <utility>

/// \brief			Element of linked pair, broken on destruction of either.
class link_element
{
public:
	link_element() :
		m_element_ptr(nullptr)
	{
	}

	link_element(link_element * element_ptr) :
		m_element_ptr(element_ptr)
	{
		if (m_element_ptr)
		{
			m_element_ptr->m_element_ptr = this;
		}
	}

	link_element(const link_element & other) = delete;

	link_element(link_element && other) noexcept
	{
		m_element_ptr = other.m_element_ptr;
		other.m_element_ptr = nullptr;

		if (m_element_ptr)
		{
			m_element_ptr->m_element_ptr = this;
		}
	}

	~link_element()
	{
		if (m_element_ptr)
		{
			m_element_ptr->m_element_ptr = nullptr;
			m_element_ptr = nullptr;
		}
	}

	link_element & operator  =(const link_element & other) = delete;

	link_element & operator  =(link_element && other) noexcept
	{
		m_element_ptr = other.m_element_ptr;
		other.m_element_ptr = nullptr;

		if (m_element_ptr)
		{
			m_element_ptr->m_element_ptr = this;
		}

		return (*this);
	}

	/// \brief			Elements' link.
	/// \returns		`true` if both elements are linked together; `false` otherwise.
	bool is_linked()
	{
		return m_element_ptr;
	}

private:
	link_element * m_element_ptr;
};
