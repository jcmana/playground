#pragma once

#include <queue>

namespace containers {

/// \brief			Standard queue with its number of elements limited.
template<class T>
class limited_queue
{
public:
	/// \brief			Constructs limited_deque with its size limited to `limit`.
	/// \param[in]		limit		Maximum number of elements.
	limited_queue(std::size_t limit);

	/// \brief			Changes the limit.
	/// \param[in]		limit		New queue limit.
	///
	/// If the limit is less than the current one, removes all the elements from beginning
	/// of the queue that don't fit.
	void limit(std::size_t limit);

	/// \brief			Returns the current limit.
	std::size_t limit() const;

	/// \brief			Returns number of elements.
	std::size_t size() const;

	/// \brief			Access the first element.
	T & front();

	/// \brief			Access the last element.
	T & back();

	/// \brief			Adds an element to the end.
	/// \param[in]		value		The value of the element to append.
	bool push(const T & value);

	/// \brief			Removes the first element.
	void pop();

private:
	/// \brief			
	std::size_t m_limit;
	std::queue<T> m_queue;
};

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
	while (limit < m_queue.size())
	{
		m_queue.pop();
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
	return m_queue.size();
}

template<class T>
T & 
limited_queue<T>::front()
{
	return m_queue.front();
}

template<class T>
T & 
limited_queue<T>::back()
{
	return m_queue.back();
}

template<class T>
bool 
limited_queue<T>::push(const T & value)
{
	if (m_queue.size() == m_limit)
	{
		return false;
	}

	m_queue.push(value);
	return true;
}

template<class T>
void 
limited_queue<T>::pop()
{
	m_queue.pop();
}

} // namespace containers