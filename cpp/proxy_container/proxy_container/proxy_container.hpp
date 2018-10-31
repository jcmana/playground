#pragma once

template <typename Container, typename T>
struct proxy_container
{
	struct proxy_iterator
	{
		proxy_iterator(typename Container::iterator it) :
			m_iterator(it)
		{
		}

		proxy_iterator & operator ++()
		{
			++m_iterator;
			return (*this);
		}

		T operator *()
		{
			return T(*m_iterator);
		}

		bool operator ==(const proxy_iterator & other)
		{
			return (m_iterator == other.m_iterator);
		}

		bool operator !=(const proxy_iterator & other)
		{
			return (m_iterator != other.m_iterator);
		}

		typename Container::iterator m_iterator;
	};

	proxy_container(Container & container) :
		m_container(container)
	{
	}

	proxy_iterator begin()
	{

		return proxy_iterator(m_container.begin());
	}

	proxy_iterator end()
	{
		return proxy_iterator(m_container.end());
	}

	Container & m_container;
};
