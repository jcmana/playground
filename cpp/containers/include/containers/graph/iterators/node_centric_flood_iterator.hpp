#pragma once

#include <list>

namespace containers {
namespace graph {


template <typename Node>
class flood_iterator
{
public:
	flood_iterator()
	{
	}

	flood_iterator(Node * start)
	{
		m_stack.push_back(start);
	}

/*
	bool operator ==(flood_iterator & other)
	{
		return (m_stack == other.m_stack);
	}

	bool operator !=(flood_iterator & other)
	{
		return !(operator ==(other));
	}
*/

	operator bool()
	{
		return (m_stack.size() > 0);
	}

	flood_iterator & operator ++()
	{
		auto * current_node = m_stack.front();

		for (auto * outgoing_edge : current_node->outgoing)
		{
			auto * edge_target_node = outgoing_edge->target;

			// add m_trail record for next recursion
			m_stack.push_back(edge_target_node);
		}

		// remove this node from queue
		m_stack.pop_front();

		return (*this);
	}

	Node * operator ->()
	{
		return m_stack.front();
	}

	Node & operator *()
	{
		return (*m_stack.front());
	}

	Node * get()
	{
		return m_stack.front();
	}

private:
	std::list<Node *> m_stack;
};


} // namespace graph
} // namespace containers
