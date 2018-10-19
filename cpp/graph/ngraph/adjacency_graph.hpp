#pragma once

#include <memory>
#include <set>
#include <map>
#include <utility>

#include "adjacency_graph_node.hpp"
#include "adjacency_graph_edge.hpp"

/// \brief		Graph specialization for edge traversing and adjacenct enumeration.
/// \desc		Graph specialization supporting:
///					- fast incoming/outgoing edges access
///					- directed edges
///					- custom node property
///					- custom edge property
template<typename NodeProperty, typename EdgeProperty>
class adjacency_graph
{
public:
	/// \brief		Node property owning type.
	using node_owner_type = adjacency_graph_node<NodeProperty>;
	/// \brief		Edge property owning type.
	using edge_owner_type = adjacency_graph_edge<EdgeProperty>;
	/// \brief		Edge owning type.
	using edge_type = std::pair<node_owner_type *, edge_owner_type *>;

public:
	// modifiers:

	/// \brief		Creates node property owner.
	node_owner_type * create_node(const NodeProperty & property);

	/// \brief		Creates edge property owner.
	edge_owner_type * create_edge(const EdgeProperty & property);

	/// \brief		Adds directed edge from source to target with edge property.
	void add_edge(node_owner_type * source, node_owner_type * target, edge_owner_type * edge);

	// enumerations:

	/// \brief		Enumerate all nodes.
	std::set<node_owner_type *> nodes();

	/// \brief		Enumerate all outgoing edges from given node.
	std::set<edge_type> outgoing(node_owner_type * source);

	/// \brief		Enumerate all outgoing edges from given node with specific edge property.
	std::set<edge_type> outgoing(node_owner_type * source, const EdgeProperty & property);

	/// \brief		Enumerate all incoming edges to given node.
	std::set<edge_type> incoming(node_owner_type * target);

	/// \brief		Enumerate all incoming edges to given node with specific edge property.
	std::set<edge_type> incoming(node_owner_type * target, const EdgeProperty & property);

	// traversions:

	void flood_traverse(node_owner_type * from, const EdgeProperty & follow_property);

private:
	node_owner_type * create_node_owner(const NodeProperty & property);

	edge_owner_type * create_edge_owner(const EdgeProperty & property);

private:
	std::set<std::unique_ptr<node_owner_type>> m_node_owners;
	std::set<std::unique_ptr<edge_owner_type>> m_edge_owners;

	std::map<node_owner_type *, std::set<edge_type>> m_edges_incoming;
	std::map<node_owner_type *, std::set<edge_type>> m_edges_outgoing;
};


template<typename NodeProperty, typename EdgeProperty>
typename adjacency_graph<NodeProperty, EdgeProperty>::node_owner_type * 
adjacency_graph<NodeProperty, EdgeProperty>::create_node(const NodeProperty & property)
{
	return create_node_owner(property);
}

template<typename NodeProperty, typename EdgeProperty>
typename adjacency_graph<NodeProperty, EdgeProperty>::edge_owner_type *
adjacency_graph<NodeProperty, EdgeProperty>::create_edge(const EdgeProperty & property)
{
	return create_edge_owner(property);
}

template<typename NodeProperty, typename EdgeProperty>
void 
adjacency_graph<NodeProperty, EdgeProperty>::add_edge(node_owner_type * source, node_owner_type * target, edge_owner_type * edge)
{
	m_edges_outgoing[source].emplace(std::make_pair(target, edge));
	m_edges_incoming[target].emplace(std::make_pair(source, edge));
}

template<typename NodeProperty, typename EdgeProperty>
std::set<typename adjacency_graph<NodeProperty, EdgeProperty>::node_owner_type *>
adjacency_graph<NodeProperty, EdgeProperty>::nodes()
{
	std::set<adjacency_graph::node_owner_type *> result;

	for (auto & node : m_node_owners)
	{
		result.insert(node.get());
	}

	return result;
}

template<typename NodeProperty, typename EdgeProperty>
std::set<typename adjacency_graph<NodeProperty, EdgeProperty>::edge_type>
adjacency_graph<NodeProperty, EdgeProperty>::outgoing(node_owner_type * source)
{
	std::set<adjacency_graph::edge_type> result;

	for (auto & edge : m_edges_outgoing[source])
	{
		result.insert(edge);
	}

	return result;
}

template<typename NodeProperty, typename EdgeProperty>
std::set<typename adjacency_graph<NodeProperty, EdgeProperty>::edge_type>
adjacency_graph<NodeProperty, EdgeProperty>::outgoing(node_owner_type * source, const EdgeProperty & property)
{
	std::set<adjacency_graph::edge_type> result;

	for (auto & edge : m_edges_outgoing[source])
	{
		if (edge.second->property() == property)
		{
			result.insert(edge);
		}
	}

	return result;
}

template<typename NodeProperty, typename EdgeProperty>
std::set<typename adjacency_graph<NodeProperty, EdgeProperty>::edge_type>
adjacency_graph<NodeProperty, EdgeProperty>::incoming(node_owner_type * target)
{
	std::set<adjacency_graph::edge_type> result;

	for (auto & edge : m_edges_incoming[target])
	{
		result.insert(edge);
	}

	return result;
}

template<typename NodeProperty, typename EdgeProperty>
std::set<typename adjacency_graph<NodeProperty, EdgeProperty>::edge_type>
adjacency_graph<NodeProperty, EdgeProperty>::incoming(node_owner_type * target, const EdgeProperty & property)
{
	std::set<adjacency_graph::edge_type> result;

	for (auto & edge : m_edges_incoming[target])
	{
		if (edge.second->property() == property)
		{
			result.insert(edge);
		}
	}

	return result;
}

template<typename NodeProperty, typename EdgeProperty>
void 
adjacency_graph<NodeProperty, EdgeProperty>::flood_traverse(node_owner_type * from, const EdgeProperty & follow_property)
{
	// user defined context
	struct stack_context
	{
		void * up_scene_node;
	};

	// stack record with context (probably templated later)
	struct stack_record
	{
		node_owner_type * node;
		stack_context context;
	};

	std::list<stack_record> stack;
	stack.push_back({ from, { nullptr }});

	while (stack.size())
	{
		auto & current_record = stack.front();
		auto & current_node = current_record.node;

		stack_context current_context = current_record.context;

		// event: visiting unique-path node
		//handle_node(current_node, current_context);
		std::cout << current_node->property() << std::endl;

		for (auto & outgoing_edge : outgoing(current_record.node, follow_property))
		{
			auto & edge_target_node = outgoing_edge.first;
			auto & edge_type = outgoing_edge.second;

			// build stack record for target node
			stack_record r;
			r.node = edge_target_node;
			r.context = current_context;

			// add stack record for next recursion
			stack.push_back(r);
		}

		// remove this node from queue
		stack.pop_front();
	}
}

template<typename NodeProperty, typename EdgeProperty>
typename adjacency_graph<NodeProperty, EdgeProperty>::node_owner_type *
adjacency_graph<NodeProperty, EdgeProperty>::create_node_owner(const NodeProperty & property)
{
	auto p_node_owner = new node_owner_type(property);
	m_node_owners.emplace(std::unique_ptr<node_owner_type>(p_node_owner));
	return p_node_owner;
}

template<typename NodeProperty, typename EdgeProperty>
typename adjacency_graph<NodeProperty, EdgeProperty>::edge_owner_type *
adjacency_graph<NodeProperty, EdgeProperty>::create_edge_owner(const EdgeProperty & property)
{
	auto p_edge_owner = new edge_owner_type(property);
	m_edge_owners.emplace(std::unique_ptr<edge_owner_type>(p_edge_owner));
	return p_edge_owner;
}
