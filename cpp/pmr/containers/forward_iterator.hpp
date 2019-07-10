#pragma once

#include <memory>
#include <iterator>

#include "forward_iterator_facade.h"

namespace pmr {
namespace containers {

/// \brief		Polymorphic forward iterator.
template<typename T>
class forward_iterator
{
public:
	/// \brief		Forward iterator facade class.
	///
	/// Forward iterator facade is interface expected to implement actual
	/// iterator functionality. `forward_iterator` practically serves as
	/// mapping of `facade` methods onto typical iterator methods.
	using facade = forward_iterator_facade<T>;

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
	/// \brief		Constructs `forward_iterator` copy.
	forward_iterator(const forward_iterator & other);
	/// \brief		Moves `other` `forward_iterator`.
	forward_iterator(forward_iterator && other) = default;

	forward_iterator & operator  =(const forward_iterator & other);
	forward_iterator & operator  =(forward_iterator && other);

	/// \brief		Comparison for equality.
	template<typename U>
	friend bool operator ==(const forward_iterator<U> & lhs, const forward_iterator<U> & rhs);
	
	/// \brief		Comparison for in-equality.
	template<typename U>
	friend bool operator !=(const forward_iterator<U> & lhs, const forward_iterator<U> & rhs);

	/// \brief		Increments the `forward_iterator`.
	forward_iterator & operator ++();
	/// \brief		Preincrements the `forward_iterator`.
	forward_iterator operator ++(int);

	/// \brief		Member access operator.
	T * operator ->();
	/// \brief		Dereference operator.
	T & operator  *();

private:
	/// \brief		Forward iterator facade implementation. Empty pointer means invalid iterator.
	std::unique_ptr<facade> m_up_iterator;
};

#pragma region forward_iterator implementation:

template<typename T>
forward_iterator<T>::forward_iterator(std::unique_ptr<facade> up_iterator) :
	m_up_iterator(std::move(up_iterator))
{
}

template<typename T>
forward_iterator<T>::forward_iterator(const forward_iterator & other) :
	m_up_iterator(other.m_up_iterator->copy())
{
}

template<typename T>
forward_iterator<T> & forward_iterator<T>::operator  =(const forward_iterator & other)
{
	m_up_iterator = other.m_up_iterator->copy();
	return (*this);
}

template<typename T>
forward_iterator<T> & forward_iterator<T>::operator  =(forward_iterator && other)
{
	m_up_iterator = std::move(other.m_up_iterator);
	return (*this);
}

template<typename U>
bool
operator ==(const forward_iterator<U> & lhs, const forward_iterator<U> & rhs)
{
	return (*lhs.m_up_iterator).equal(*rhs.m_up_iterator);
}

template<typename U>
bool
operator !=(const forward_iterator<U> & lhs, const forward_iterator<U> & rhs)
{
	return !(lhs == rhs);
}

template<typename T>
forward_iterator<T> &
forward_iterator<T>::operator ++()
{
	m_up_iterator->next();
	return (*this);
}

template<typename T>
forward_iterator<T> 
forward_iterator<T>::operator ++(int)
{
	auto that = (*this);
	m_up_iterator->next();
	return that;
}

template<typename T>
T *
forward_iterator<T>::operator ->()
{
	return (&m_up_iterator->read());
}

template<typename T>
T &
forward_iterator<T>::operator  *()
{
	return m_up_iterator->read();
}

#pragma endregion


} // namespace containers
} // namespace pmr
