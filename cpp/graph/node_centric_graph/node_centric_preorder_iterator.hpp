#pragma once

#include <cassert>
#include <list>

template <typename Graph>
class preorder_iterator
{
public:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

public:
	preorder_iterator() :
		m_node_ptr(nullptr)
	{
	}

	preorder_iterator(node * node_ptr) :
		m_node_ptr(node_ptr)
	{
		m_trail.push_front({ m_node_ptr, node_ptr->outgoing.begin(), node_ptr->outgoing.end() });
	}

	bool operator ==(preorder_iterator & other)
	{
		return (m_node_ptr == other.m_node_ptr && m_trail == other.m_trail);
	}

	bool operator !=(preorder_iterator & other)
	{
		return !(operator ==(other));
	}

	preorder_iterator & operator ++()
	{
#ifdef _DEBUG
		assert(m_trail.size() > 0 && "Iterator is not incrementable");
#endif

		// emerge
		while (m_trail.front().it_current == m_trail.front().it_end)
		{
			m_trail.pop_front();

			if (m_trail.size() == 0)
			{
				m_node_ptr = nullptr;
				return (*this);
			}
		}

		// current edge and node
		auto & it_current = m_trail.front().it_current;
		auto * edge_ptr = (*it_current);
		auto * node_ptr = edge_ptr->target;
		m_node_ptr = node_ptr;

		// TODO: debug check: graph cycles (for each visited node check if it is already on stack)

		// submerge (if not leaf)
		if (node_ptr->outgoing.size() > 0)
		{
			m_trail.push_front({ m_node_ptr, node_ptr->outgoing.begin(), node_ptr->outgoing.end() });
		}

		// increment
		++it_current;

		return (*this);
	}

	node * operator ->()
	{
		return m_node_ptr;
	}

	node * operator *()
	{
		return m_node_ptr;
	}

private:
	using node_iterator = typename graph::node_container::iterator;
	using edge_iterator = typename graph::edge_container::iterator;

	struct breadcrumb
	{
		node * node_ptr;
		edge_iterator it_current;
		edge_iterator it_end;
	};

private:
	node * m_node_ptr;
	std::list<breadcrumb> m_trail;
};