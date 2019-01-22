#pragma once

#include <queue>

namespace containers {

/// \brief			Standard queue with its number of elements limited.
template<class T>
class limited_queue
{
public:
	/// \brief			Constructs limited_deque with its size limited to `limit`.
	/// \param[in]		limit		Maximum number of stored items.
	limited_queue(std::size_t limit);

	/// \brief			Access the first element.
	T & front();

	/// \brief			Access the last element.
	T & back();

	/// \brief			Adds an element to the end.
	/// \param[in]		value		The value of the element to append.
	bool push(const T & value);

	/// \brief			Removes the first element 
	void pop();

	std::size_t size() const;

private:
	std::size_t m_limit;
	std::queue<T> m_queue;
};

template<class T>
limited_queue<T>::limited_queue(std::size_t limit) :
	m_limit(limit)
{
}

template<class T>
T & limited_queue<T>::front()
{
	return m_queue.front();
}

template<class T>
T & limited_queue<T>::back()
{
	return m_queue.back();
}

template<class T>
bool limited_queue<T>::push(const T & value)
{
	if (m_queue.size() == m_limit)
	{
		return false;
	}

	m_queue.push(value);
	return true;
}

template<class T>
void limited_queue<T>::pop()
{
	m_queue.pop();
}

template<class T>
std::size_t limited_queue<T>::size() const
{
	return m_queue.size();
}


} // namespace containers