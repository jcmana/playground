#pragma once

#include "../cursors/path_cursor.hpp"

namespace containers {
namespace graph {

template<typename G>
class postorder_path_iterator
{
public:
	postorder_path_iterator(G & graph) :
		m_cursor(graph)
	{
	}

	postorder_path_iterator(G & graph, typename G::node node) :
		m_cursor(graph, std::vector<typename G::edge>(graph[node].outgoing.rbegin(), graph[node].outgoing.rend()))
	{
		while (m_cursor.match() == false)
		{
			m_cursor.expand();
		}
	}

	postorder_path_iterator(G & graph, typename G::edge edge) :
		m_cursor(graph, edge)
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

	const std::vector<typename G::edge> & operator  *() const
	{
		return *m_cursor;
	}

	const std::vector<typename G::edge> * operator ->() const
	{
		return &*m_cursor;
	}

private:
	path_cursor<G> m_cursor;
};

} // namespace graph
} // namespace containers
