#pragma once

#include <utility>

/// \brief			Pair of linked elements, broken on destruction of either.
class pair_element
{
public:
	pair_element() :
		m_element_ptr(nullptr)
	{
	}

	pair_element(pair_element * element_ptr) :
		m_element_ptr(element_ptr)
	{
		if (m_element_ptr)
		{
			m_element_ptr->m_element_ptr = this;
		}
	}

	pair_element(const pair_element & other) = delete;

	pair_element(pair_element && other) noexcept
	{
		m_element_ptr = other.m_element_ptr;
		other.m_element_ptr = nullptr;

		if (m_element_ptr)
		{
			m_element_ptr->m_element_ptr = this;
		}
	}

	~pair_element()
	{
		if (m_element_ptr)
		{
			m_element_ptr->m_element_ptr = nullptr;
			m_element_ptr = nullptr;
		}
	}

	pair_element & operator  =(const pair_element & other) = delete;

	pair_element & operator  =(pair_element && other) noexcept
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
	/// \returns		`true` if both elements are alive; `false` otherwise.
	bool linked()
	{
		return m_element_ptr;
	}

private:
	pair_element * m_element_ptr;
};
