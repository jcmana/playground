#pragma once

#include "../cursors/path_cursor.hpp"

namespace containers {
namespace graph {

template<typename G>
class preorder_path_iterator
{
public:
	preorder_path_iterator(G & graph) :
		m_cursor(graph)
	{
	}

	preorder_path_iterator(G & graph, typename G::node node) :
		m_cursor(graph, std::vector<typename G::edge>(graph[node].outgoing.rbegin(), graph[node].outgoing.rend()))
	{
	}

	preorder_path_iterator(G & graph, typename G::edge edge) :
		m_cursor(graph, edge)
	{
	}

	/// \brief		Finds next path in postorder traversal.
	preorder_path_iterator & operator ++()
	{
		const auto cursor_depth = m_cursor.depth();

		for (std::size_t n = 0; n < cursor_depth && m_cursor.match(); ++n)
		{
			m_cursor.consume();
		}

		if (m_cursor.empty() == false)
		{
			m_cursor.expand();
		}

		return (*this);
	}

	bool operator ==(const preorder_path_iterator & other) const
	{
		return m_cursor == other.m_cursor;
	}

	bool operator !=(const preorder_path_iterator & other) const
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

	/// \brief		Skips entire subtree.
	void skip()
	{
		const auto cursor_depth = m_cursor.depth();

		for (std::size_t n = 0; n < cursor_depth && !m_cursor.match(); ++n)
		{
			m_cursor.skip();
		}
	}

private:
	path_cursor<G> m_cursor;
};

} // namespace graph
} // namespace containers
