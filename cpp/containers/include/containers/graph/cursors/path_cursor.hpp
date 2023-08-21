#pragma once

#include <cassert>
#include <vector>

namespace containers {
namespace graph {

/// \brief		Dual-stack graph traversal.
template<typename G>
class path_cursor
{
public:
	path_cursor(G & graph) :
		m_graph(graph)
	{
	}

	path_cursor(G & graph, std::vector<typename G::edge> edges) :
		m_graph(graph),
		m_stack_down(edges)
	{
		expand();
	}

	/// \brief		Skips next subtree expansion.
	void skip()
	{
		m_stack_down.pop_back();
	}

	/// \brief		Removes last node from both stacks.
	void consume()
	{
		m_stack_down.pop_back();
		m_stack_path.pop_back();
	}

	/// \brief		Expands last node from expansion stack.
	void expand()
	{
		const auto edge = m_stack_down.back();
		const auto node = m_graph[edge].target;

		const auto & edge_storage = m_graph[edge];
		const auto & node_storage = m_graph[node];

		// Expand in reverse order (to visit leftmost subtrees first)
		for (auto it = node_storage.outgoing.rbegin(); it < node_storage.outgoing.rend(); ++it)
		{
			m_stack_down.push_back(*it);
		}

#ifndef NDEBUG
        /*
		// Check if adding current edge creates a cycle
		{
			const auto pred = [](typename G::edge edge)
			{
				return edge.offset == edge.offset;
			};
			const auto it = std::find_if(m_stack_path.begin(), m_stack_path.end(), pred);

			assert(it == m_stack_path.end() && "Graph cycle detected.");
		}
        */
#endif

		// Push the expanded edge onto path stack
		m_stack_path.push_back(edge);
	}

	bool operator ==(const path_cursor & other) const
	{
		if (&m_graph != &other.m_graph)
		{
			return false;
		}

		if (m_stack_down.size() != other.m_stack_down.size())
		{
			return false;
		}

		if (m_stack_path.size() != other.m_stack_path.size())
		{
			return false;
		}

		if (m_stack_down.empty() == other.m_stack_down.empty())
		{
			return true;
		}

		if (m_stack_path.empty() == other.m_stack_path.empty())
		{
			return true;
		}

		// Full comparison of down stacks:
		{
			auto it_down_this = m_stack_down.begin();
			auto it_down_that = other.m_stack_down.begin();

			for (; it_down_this != m_stack_down.end(); ++it_down_this, ++it_down_that)
			{
				if (it_down_this->offset != it_down_that->offset)
				{
					return false;
				}
			}
		}

		// Full comparison of path stacks:
		{
			auto it_path_this = m_stack_path.begin();
			auto it_path_that = other.m_stack_path.begin();

			for (; it_path_this != m_stack_path.end(); ++it_path_this, ++it_path_that)
			{
				if (it_path_this->offset != it_path_that->offset)
				{
					return false;
				}
			}
		}

		return true;
	}

	bool operator !=(const path_cursor & other) const
	{
		return !((*this) == other);
	}

	const std::vector<typename G::edge> & operator  *() const
	{
		return m_stack_path;
	}

	const std::vector<typename G::edge> * operator ->() const
	{
		return &m_stack_path;
	}

	/// \brief			Expansion stack is empty.
	bool empty() const
	{
		return m_stack_down.empty();
	}

	/// \brief			Expansion and path stack last nodes match.
	bool match() const
	{
		return m_stack_down.back().offset == m_stack_path.back().offset;
	}

	/// \brief			Path stack depth.
	std::size_t depth() const
	{
		return m_stack_path.size();
	}

private:
	G & m_graph;

	std::vector<typename G::edge> m_stack_down;
	std::vector<typename G::edge> m_stack_path;
};

} // namespace graph
} // namespace containers
