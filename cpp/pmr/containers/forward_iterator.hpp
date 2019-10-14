#pragma once

#include <memory>
#include <iterator>

//#include "forward_iterator_facade.h"

namespace pmr {
namespace containers {

/// \brief		Polymorphic forward iterator.
///
/// Functionality is hidden behind `facade` and this class exposes the well-known
/// forward iterator API. `facade` is runtime dependent and therefore conformant with
/// substitution principle.
template<typename T>
class forward_iterator
{
public:
	/// \brief		Forward iterator facade class.
	///
	/// Forward iterator facade is interface expected to implement actual
	/// iterator functionality.
	class facade
	{
	public:
		virtual ~facade() = default;

		/// \brief		Move to the next element.
		virtual void next() = 0;
		/// \brief		Access current element.
		virtual T & read() = 0;
		/// \brief		Compares for equality.
		virtual bool equal(const facade & other) const = 0;
		/// \brief		Obtains iterator copy.
		virtual std::unique_ptr<facade> copy() const = 0;
	};

	// Standard library compliance aliases:
	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using pointer = T *;
	using reference = T &;

public:
	/// \brief		Constructs empty (invalid) `forward_iterator`.
	forward_iterator() = default;
	/// \brief		Constructs `forward_iterator` from `facade`.
	forward_iterator(std::unique_ptr<facade> up_iterator);
	/// \brief		Copies from `other` `forward_iterator`.
	forward_iterator(const forward_iterator & other);
	/// \brief		Moves from `other` `forward_iterator`.
	forward_iterator(forward_iterator && other) = default;

	forward_iterator & operator  =(const forward_iterator & other);
	forward_iterator & operator  =(forward_iterator && other);

	/// \brief		Comparison for equality.
	template<typename T>
	friend bool operator ==(const forward_iterator<T> & lhs, const forward_iterator<T> & rhs);
	
	/// \brief		Comparison for in-equality.
	template<typename T>
	friend bool operator !=(const forward_iterator<T> & lhs, const forward_iterator<T> & rhs);

	/// \brief		Increments the `forward_iterator`.
	forward_iterator & operator ++();
	/// \brief		Preincrements the `forward_iterator`.
	forward_iterator operator ++(int);

	/// \brief		Member access operator.
	T * operator ->();
	/// \brief		Dereference operator.
	T & operator  *();

private:
	/// \brief		Forward iterator `facade`. Empty pointer means invalid iterator.
	std::unique_ptr<facade> m_up_facade;
};

#pragma region forward_iterator implementation:

template<typename T>
forward_iterator<T>::forward_iterator(std::unique_ptr<facade> up_iterator) :
	m_up_facade(std::move(up_iterator))
{
}

template<typename T>
forward_iterator<T>::forward_iterator(const forward_iterator & other) :
	m_up_facade(other.m_up_facade->copy())
{
}

template<typename T>
forward_iterator<T> & 
forward_iterator<T>::operator  =(const forward_iterator & other)
{
	m_up_facade = other.m_up_facade->copy();
	return (*this);
}

template<typename T>
forward_iterator<T> & 
forward_iterator<T>::operator  =(forward_iterator && other)
{
	m_up_facade = std::move(other.m_up_facade);
	return (*this);
}

template<typename T>
bool
operator ==(const forward_iterator<T> & lhs, const forward_iterator<T> & rhs)
{
	return (*lhs.m_up_facade).equal(*rhs.m_up_facade);
}

template<typename T>
bool
operator !=(const forward_iterator<T> & lhs, const forward_iterator<T> & rhs)
{
	return !(lhs == rhs);
}

template<typename T>
forward_iterator<T> &
forward_iterator<T>::operator ++()
{
	m_up_facade->next();
	return (*this);
}

template<typename T>
forward_iterator<T> 
forward_iterator<T>::operator ++(int)
{
	auto that = (*this);
	m_up_facade->next();
	return that;
}

template<typename T>
T *
forward_iterator<T>::operator ->()
{
	return (&m_up_facade->read());
}

template<typename T>
T &
forward_iterator<T>::operator  *()
{
	return m_up_facade->read();
}

#pragma endregion


} // namespace containers
} // namespace pmr
