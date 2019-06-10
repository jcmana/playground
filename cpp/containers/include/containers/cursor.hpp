#pragma once

namespace containers {

/// \brief		Range-aware iterator.
template<typename I>
class cursor
{
public:
	cursor() = default;
	cursor(const I & begin, const I & end, const I & current);

	operator bool() const;

	template<typename I>
	friend bool operator ==(const cursor<I> & left, const cursor<I> & right);
	template<typename I>
	friend bool operator !=(const cursor<I> & left, const cursor<I> & right);

	cursor & operator ++();
	cursor & operator --();

	typename I::pointer operator ->();
	typename I::reference operator  *();

	/// \brief		Restarts the cursor to point to the first element = begin().
	cursor & restart();
	/// \brief		Finishes the cursor to point past the last element = end().
	cursor & finish();

private:
	I m_begin;
	I m_end;

	I m_current;
};

#pragma region cursor implementation:

template<typename I>
cursor<I>::cursor(const I & begin, const I & end, const I & current) :
	m_begin(begin),
	m_end(end),
	m_current(current)
{
}

template<typename I>
cursor<I>::operator bool() const
{
	return (m_current != m_end);
}

template<typename I>
bool 
operator ==(const cursor<I> & left, const cursor<I> & right)
{
	return (left.m_begin == right.m_begin && left.m_end == right.m_end && left.m_current == right.m_current);
}

template<typename I>
bool 
operator !=(const cursor<I> & left, const cursor<I> & right)
{
	return !(left == right);
}

template<typename I>
cursor<I> & 
cursor<I>::operator ++()
{
	m_current.operator ++();
	return (*this);
}

template<typename I>
cursor<I> &
cursor<I>::operator --()
{
	m_current.operator --();
	return (*this);
}

template<typename I>
typename I::pointer
cursor<I>::operator ->()
{
	return m_current.operator ->();
}

template<typename I>
typename I::reference
cursor<I>::operator  *()
{
	return	m_current.operator  *();
}

template<typename I>
cursor<I> & 
cursor<I>::restart()
{
	m_current = m_begin;
	return (*this);
}

template<typename I>
cursor<I> & 
cursor<I>::finish()
{
	m_current = m_end;
	return (*this);
}

#pragma endregion

} // namespace containers