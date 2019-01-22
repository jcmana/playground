#pragma once

#include <deque>
#include <utility>

namespace containers {

/// \brief			Standard queue with its number of elements limited.
template<class T>
class nonuniform_limited_queue
{
public:
	/// \brief			Constructs limited_deque with its size limited to `volume_limit`.
	/// \param[in]		limit		Maximum number of elements.
	nonuniform_limited_queue(std::size_t limit);

	/// \brief			Changes the volume_limit.
	/// \param[in]		limit		New queue volume_limit.
	///
	/// If the volume_limit is less than the current one, removes all the elements from beginning
	/// of the queue that don't fit.
	void volume_limit(std::size_t limit);

	/// \brief			Returns the current volume limit.
	std::size_t volume_limit() const;

	std::size_t volume() const;

	/// \brief			Returns number of elements.
	std::size_t size() const;

	/// \brief			Access the first element.
	T & front();

	/// \brief			Access the last element.
	T & back();

	/// \brief			Adds an element to the end.
	/// \param[in]		value		The value of the element to append.
	/// \param[in]		size		Size of the item.
	bool push(const T & value, std::size_t volume);

	/// \brief			Removes the first element.
	void pop();

protected:
	std::size_t m_volume;
	std::size_t m_volume_limit;

	std::deque<std::pair<T, std::size_t>> m_deque;
};


template<class T>
nonuniform_limited_queue<T>::nonuniform_limited_queue(std::size_t limit) :
	m_volume(0),
	m_volume_limit(limit)
{
}

template<class T>
void
nonuniform_limited_queue<T>::volume_limit(std::size_t limit)
{
	// Remove the overflow from the beginning of the queue
	while (limit < m_volume)
	{
		pop();
	}

	m_volume_limit = limit;
}

template<class T>
std::size_t
nonuniform_limited_queue<T>::volume_limit() const
{
	return m_volume_limit;
}

template<class T>
std::size_t 
nonuniform_limited_queue<T>::volume() const
{
	return m_volume;
}

template<class T>
std::size_t 
nonuniform_limited_queue<T>::size() const
{
	return m_volume;
}

template<class T>
T &
nonuniform_limited_queue<T>::front()
{
	return m_deque.front().first;
}

template<class T>
T &
nonuniform_limited_queue<T>::back()
{
	return m_deque.back().first;
}

template<class T>
bool
nonuniform_limited_queue<T>::push(const T & value, std::size_t volume)
{
	if ((m_volume + volume) > m_volume_limit)
	{
		return false;
	}

	// Emplace last (element, volume) pair
	m_deque.emplace_back(value, volume);

	// Add into queue volume
	m_volume = m_volume + volume;

	return true;
}

template<class T>
void
nonuniform_limited_queue<T>::pop()
{
	std::size_t popped_element_volume = m_deque.front().second;

	// Remove first element
	m_deque.pop_front();

	// Subtract its volume from queue volume
	m_volume = m_volume - popped_element_volume;
}


} // namespace containers
