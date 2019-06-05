#pragma once

#include <memory>

#include "forward_iterator_facade.h"

namespace pmr {
namespace containers {

template<typename T>
class forward_iterator
{
public:
	using facade = forward_iterator_facade<T>;

public:
	forward_iterator();
	forward_iterator(std::unique_ptr<facade> up_iterator);
	forward_iterator(const forward_iterator & other);

	template<typename U>
	friend bool operator ==(const forward_iterator<U> & left, const forward_iterator<U> & right);
	
	template<typename U>
	friend bool operator !=(const forward_iterator<U> & left, const forward_iterator<U> & right);

	forward_iterator & operator ++();
	forward_iterator operator ++(int);

	T * operator ->();
	T & operator  *();

private:
	std::unique_ptr<facade> m_up_iterator;
};

#pragma region forward_iterator implementation:

template<typename T>
forward_iterator<T>::forward_iterator()
{
}

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

template<typename U>
bool
operator ==(const forward_iterator<U> & left, const forward_iterator<U> & right)
{
	return (*left.m_up_iterator).equal(*right.m_up_iterator);
}

template<typename U>
bool
operator !=(const forward_iterator<U> & left, const forward_iterator<U> & right)
{
	return !(left == right);
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
