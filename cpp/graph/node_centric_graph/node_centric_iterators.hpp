#pragma once

#include <list>

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

	bool operator ==(flood_iterator & other)
	{
		return (m_stack == other.m_stack);
	}

	bool operator !=(flood_iterator & other)
	{
		return !(operator ==(other));
	}

	flood_iterator & operator ++()
	{
		auto * current_node = m_stack.front();

		for (auto * outgoing_edge : current_node->outgoing)
		{
			auto * edge_target_node = outgoing_edge->target;

			// add m_stack record for next recursion
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
		return m_stack.front();
	}

private:
	std::list<Node *> m_stack;
};

template <typename Graph>
class sibling_iterator
{
public:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

	using iterator = typename graph::edge_container::iterator;

public:
	sibling_iterator(node * parent_node_ptr, iterator current) :
		m_parent_node_ptr(parent_node_ptr),
		m_current(current)
	{
	}

	bool operator ==(sibling_iterator & other)
	{
		return (m_parent_node_ptr == other.m_parent_node_ptr && m_current == other.m_current);
	}

	bool operator !=(sibling_iterator & other)
	{
		return !(operator ==(other));
	}

	sibling_iterator & operator ++()
	{
		++m_current;
		return (*this);
	}

	node * operator ->()
	{
		return (*m_current)->target;
	}

	node * operator *()
	{
		return (*m_current)->target;
	}

private:


private:
	node * m_parent_node_ptr;
	iterator m_current;
};

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
		m_stack.emplace_front(node_ptr->outgoing.begin(), node_ptr->outgoing.end());
	}

	bool operator ==(preorder_iterator & other)
	{
		return (m_node_ptr == other.m_node_ptr && m_stack == other.m_stack);
	}

	bool operator !=(preorder_iterator & other)
	{
		return !(operator ==(other));
	}

	preorder_iterator & operator ++()
	{
		// emerge
		while (m_stack.front().first == m_stack.front().second)
		{
			m_stack.pop_front();

			if (m_stack.size() == 0)
			{
				m_node_ptr = nullptr;
				return (*this);
			}
		}

		// current edge and node
		auto & it_current = m_stack.front().first;
		auto * edge_ptr = (*it_current);
		auto * node_ptr = edge_ptr->target;
		m_node_ptr = node_ptr;

		// TODO: debug check: graph cycles (for each visited node check if it is already on stack)

		// submerge (if not leaf)
		if (node_ptr->outgoing.size() > 0)
		{
			m_stack.emplace_front(node_ptr->outgoing.begin(), node_ptr->outgoing.end());
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

	using range = std::pair<edge_iterator, edge_iterator>;

private:
	node * m_node_ptr;
	std::list<range> m_stack;
};