#pragma once

#include <memory>
#include <map>
#include <set>
#include <exception>

#include "node.h"

struct in_out_graph_record
{
	using node_type = std::shared_ptr<node>;
	using container_type = std::set<node_type>;

	container_type in;
	container_type out;
};

class in_out_graph
{
public:
	using value_type = in_out_graph_record;
	using node_type = value_type::node_type;
	using node_container_type = std::set<node_type>;
	using edge_container_type = std::map<node_type, value_type>;
	using node_iterator = node_container_type::iterator;
	using edge_iterator = edge_container_type::iterator;

public:
	void add_node(const node_type & node)
	{
		m_nodes.insert(node);
	}

	void add_edge(const node_type & from, const node_type & to)
	{
		if (m_nodes.find(from) == m_nodes.end())
		{
			throw std::invalid_argument(std::string("Unknown node: '" + from->name() + "'"));
		}

		if (m_nodes.find(to) == m_nodes.end())
		{
			throw std::invalid_argument(std::string("Unknown node: '" + to->name() + "'"));
		}

		m_edges[from].out.insert(to);
		m_edges[to].in.insert(from);
	}

	const value_type::container_type & incoming(const node_type & to)
	{
		return m_edges[to].in;
	}

	const value_type::container_type & outgoing(const node_type & from)
	{
		return m_edges[from].out;
	}

	node_iterator begin_nodes()
	{
		return m_nodes.begin();
	}

	node_iterator end_nodes()
	{
		return m_nodes.end();
	}

	edge_iterator begin_edges()
	{
		return m_edges.begin();
	}

	edge_iterator end_edges()
	{
		return m_edges.end();
	}

private:
	node_container_type m_nodes;
	edge_container_type m_edges;
};
