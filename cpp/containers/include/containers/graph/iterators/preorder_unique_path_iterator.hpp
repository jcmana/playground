#pragma once

#include <iostream>

#include <vector>
#include <stack>

#include "preorder_edge_iterator.hpp"

namespace containers {
namespace graph {

template<typename Graph>
class preorder_unique_path_iterator
{
public:
	preorder_unique_path_iterator() :
		m_iterator()
	{
	}

	preorder_unique_path_iterator(typename Graph * graph_ptr) :
		m_iterator(graph_ptr)
	{
	}

	preorder_unique_path_iterator(typename Graph * graph_ptr, typename Graph::node * node_ptr) :
		m_iterator(graph_ptr, node_ptr)
	{
		m_path.push_back(*m_iterator);
	}

	/// \brief		Compares iterators for equality.
	template<typename Graph>
	friend bool operator ==(const preorder_unique_path_iterator<Graph> & left, const preorder_unique_path_iterator<Graph> & right);

	/// \brief		Compares iterators for in-equality.
	template<typename Graph>
	friend bool operator !=(const preorder_unique_path_iterator<Graph> & left, const preorder_unique_path_iterator<Graph> & right);

	preorder_unique_path_iterator & operator ++()
	{
		increment();
		return (*this);
	}

	typename preorder_iterator<Graph> & operator ->()
	{
		return m_iterator;
	}

	typename Graph::edge * operator  *()
	{
		return (*m_iterator);
	}

	const std::vector<typename Graph::edge *> & path() const
	{
		return m_path;
	}

private:
	void increment()
	{
		// Increment underlaying iterator
		++m_iterator;

		// Adjust current path
		// (find current outgoing edge in the path and remove everything above it)
		for (auto path_it = m_path.begin(); path_it != m_path.end(); ++path_it)
		{
			for (auto outgoing_it = (*path_it)->target->outgoing.begin(); outgoing_it != (*path_it)->target->outgoing.end(); ++outgoing_it)
			{
				if (*outgoing_it == *m_iterator)			// exception due to invalid pointer dereference attempt
				{
					m_path.erase(path_it + 1, m_path.end());
					m_path.push_back(*m_iterator);
					return;
				}
			}
		}

		m_path.erase(m_path.begin(), m_path.end());
		m_path.push_back(*m_iterator);
	}

private:
	preorder_iterator<Graph> m_iterator;
	std::vector<typename Graph::edge *> m_path;
};

template<typename Graph>
bool operator ==(const preorder_unique_path_iterator<Graph> & left, const preorder_unique_path_iterator<Graph> & right)
{
	return (left.m_iterator == right.m_iterator);
}

/// \brief		Compares iterators for in-equality.
template<typename Graph>
bool operator !=(const preorder_unique_path_iterator<Graph> & left, const preorder_unique_path_iterator<Graph> & right)
{
	return !(left == right);
}

} // namespace graph
} // namespace containers
