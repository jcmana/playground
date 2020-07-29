#pragma once

#include <cassert>
#include <vector>

namespace containers {
namespace graph {

template<typename G>
class preorder_path_cursor
{
public:
	preorder_path_cursor(G & graph) :
		m_graph(graph)
	{
	}

	preorder_path_cursor(G & graph, typename G::node node) :
		m_graph(graph)
	{
		if (m_graph[node].outgoing.empty())
		{
			return;
		}

		for (auto it = m_graph[node].outgoing.rbegin(); it != m_graph[node].outgoing.rend(); ++it)
		{
			m_stack_down.push_back(*it);
		}

		expand();
	}

	preorder_path_cursor(G & graph, typename G::edge edge) :
		m_graph(graph)
	{
		m_stack_down.push_back(edge);

		expand();
	}

	/// \brief		Skips next subtree expansion.
	void skip()
	{
		m_stack_down.pop_back();
	}

	void consume()
	{
		m_stack_down.pop_back();
		m_stack_path.pop_back();
	}

	void expand()
	{
		const auto e = m_stack_down.back();
		const auto n = m_graph[e].target;

		const auto & es = m_graph[e];
		const auto & ns = m_graph[n];

		for (auto it = ns.outgoing.rbegin(); it < ns.outgoing.rend(); ++it)
		{
			m_stack_down.push_back(*it);
		}

#ifndef NDEBUG
		{
			const auto pred = [e](typename G::edge edge)
			{
				return e.offset == edge.offset;
			};

			const auto it = std::find_if(m_stack_path.begin(), m_stack_path.end(), pred);
			assert(it == m_stack_path.end() && "Graph cycle detected.");
		}
#endif

		m_stack_path.push_back(e);
	}

	bool operator ==(const preorder_path_cursor & other) const
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

	bool operator !=(const preorder_path_cursor & other) const
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

	/// \brief			Stack expansion is empty.
	bool empty() const
	{
		return m_stack_down.empty() && m_stack_path.empty();
	}

	/// \brief			Stack expansion is at bottom.
	bool match() const
	{
		return m_stack_down.back().offset == m_stack_path.back().offset;
	}

	/// \brief			Depth (distance from initial `edge`).
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
