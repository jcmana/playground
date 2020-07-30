#pragma once

#include "../cursors/path_cursor.hpp"

namespace containers {
namespace graph {

template<typename G>
class postorder_path_iterator
{
public:
	postorder_path_iterator()
	{
	}

	postorder_path_iterator(typename G::node * node_ptr) :
		m_cursor({node_ptr->outgoing.rbegin(), node_ptr->outgoing.rend()})
	{
		while (m_cursor.match() == false)
		{
			m_cursor.expand();
		}
	}

	postorder_path_iterator(typename G::edge * edge_ptr) :
		m_cursor({edge_ptr})
	{
		while (m_cursor.match() == false)
		{
			m_cursor.expand();
		}
	}

	postorder_path_iterator & operator ++()
	{
		m_cursor.consume();

		while (m_cursor.empty() == false)
		{
			if (m_cursor->empty() == false && m_cursor.match())
			{
				break;
			}

			m_cursor.expand();
		}

		return (*this);
	}

	bool operator ==(const postorder_path_iterator & other) const
	{
		return m_cursor == other.m_cursor;
	}

	bool operator !=(const postorder_path_iterator & other) const
	{
		return m_cursor != other.m_cursor;
	}

	const std::vector<typename G::edge *> & operator  *() const
	{
		return *m_cursor;
	}

	const std::vector<typename G::edge *> * operator ->() const
	{
		return &*m_cursor;
	}

private:
	path_cursor<G> m_cursor;
};

} // namespace graph
} // namespace containers
