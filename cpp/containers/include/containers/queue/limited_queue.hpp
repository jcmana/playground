#pragma once

#include <deque>

namespace containers {

/// \brief			Standard queue with its number of elements limited.
template<class T>
class limited_queue
{
public:
	/// \brief			Constructs `limited_queue` with maximum size volume_limit (implementation specific).
	limited_queue();

	/// \brief			Constructs `limited_queue` with its size limited to `volume_limit`.
	/// \param[in]		volume_limit		Maximum number of elements.
	limited_queue(std::size_t limit);

	/// \brief			Changes the volume_limit.
	/// \param[in]		volume_limit		New queue volume_limit.
	///
	/// If the volume_limit is less than the current one, removes all the elements from beginning
	/// of the queue that don't fit.
	void limit(std::size_t limit);

	/// \brief			Returns the current volume_limit.
	std::size_t limit() const;

	/// \brief			Returns number of elements.
	std::size_t size() const;

	/// \brief			Access the first element.
	T & front();

	/// \brief			Access the last element.
	T & back();

	/// \brief			Adds an element to the end.
	/// \param[in]		value		The value of the element to append.
	/// \returns		false, if volume_limit is reached; true, if element was stored.
	bool push(const T & value);

	/// \brief			Removes the first element.
	void pop();

protected:
	std::size_t m_limit;
	std::deque<T> m_deque;
};

template<class T>
limited_queue<T>::limited_queue() :
	m_limit(m_deque.max_size())
{
}

template<class T>
limited_queue<T>::limited_queue(std::size_t limit) :
	m_limit(limit)
{
}

template<class T>
void 
limited_queue<T>::limit(std::size_t limit)
{
	// Remove the overflow from the beginning of the queue
	while (limit < m_deque.size())
	{
		pop();
	}

	m_limit = limit;
}

template<class T>
std::size_t 
limited_queue<T>::limit() const
{
	return m_limit;
}

template<class T>
std::size_t
limited_queue<T>::size() const
{
	return m_deque.size();
}

template<class T>
T & 
limited_queue<T>::front()
{
	return m_deque.front();
}

template<class T>
T & 
limited_queue<T>::back()
{
	return m_deque.back();
}

template<class T>
bool 
limited_queue<T>::push(const T & value)
{
	if (m_deque.size() == m_limit)
	{
		return false;
	}

	m_deque.push_back(value);
	return true;
}

template<class T>
void 
limited_queue<T>::pop()
{
	m_deque.pop_front();
}

} // namespace containers