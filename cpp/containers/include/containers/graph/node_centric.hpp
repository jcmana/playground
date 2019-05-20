#pragma once

#include <cassert>
#include <vector>
#include <map>
#include <utility>

namespace containers {
namespace graph {

/// \brief		Directed graph with node and edges properties.
///
/// \par Properties:
///		- directed edges
///		- O(1) incoming/outgoing edges access
///		- O(1) custom node property access
///		- O(1) custom edge property access
///
/// Graph itself doesn't support any typical traverses or algorithms, therefore serves exclusivelly
/// as underlaying structure holding the information about nodes and edges and allows basic iteration
/// over those.
///
///	Nodes and edges are managed using `node_centric_graph::node` and `node_centric_graph::edge` classes.
/// These are created exclusivelly by calling `create_node()` or `create_edge()` methods and their lifetime
/// is managed by `node_centric_graph` class (meaning, it is illegal to delete these pointers manually).
///
///	\par Example:
/// \code
/// // Include graph header
///	#include <node_centric_graph.hpp>
///
///	// Create the graph object with required properties (std::string node property, int edge property)
/// using graph = node_centric_graph<std::string, int>;
/// graph g;
///
/// // Create some nodes:
/// graph::node * node_a = g.create_node("a");
/// graph::node * node_b = g.create_node("b");
/// graph::node * node_c = g.create_node("c");
///
/// // Create edges between nodes:
/// graph::edge * edge_ab = g.create_edge(node_a, node_b, 23);
/// graph::edge * edge_bc = g.create_edge(node_b, node_c, 27);
/// graph::edge * edge_cb = g.create_edge(node_c, node_b, 6);
///
/// // Print all nodes:
/// std::cout << "Graph nodes:" << std::endl;
/// 
/// for (auto & node : g.nodes)
/// {
/// 	std::cout << node->property << std::endl;
/// }
/// 
/// // Find edge with minimal property:
/// auto it_min = std::min_element(g.edges.begin(), g.edges.end(), [](const graph::edge * a, const graph::edge * b) -> bool
/// {
/// 	return (a->property > b->property);
/// });
///
///	// Find node with specific property and create edge from it:
/// auto it_find = std::find_if(g.nodes.begin(), g.nodes.end(), [](const graph::node * node) -> bool
/// {
/// 	return ("a" == node->property);
/// });
/// 
/// if (it_find != g.nodes.end())
/// {
/// 	g.create_edge(*it_find, node_c, 7);
/// }
/// \endcode
template <typename NodeProperty, typename EdgeProperty>
class node_centric
{
public:
	class node;
	class edge;

	/// \brief		Base container type for storing graph elements.
	template<typename Element>
	using container = std::vector<Element>;

	using node_container = container<node *>;
	using edge_container = container<edge *>;

public:
	/// \brief		Storage for the node property and outgoing/incoming edges.
	class node
	{
	public:
		/// \brief		Outgoing `edge`s.
		edge_container outgoing;
		/// \brief		Incoming `edge`s.
		edge_container incoming;
		/// \brief		Node property.
		NodeProperty property;

	protected:
		template <typename ... Args>
		node(Args && ... node_property_args);

	friend node_centric;
	};

	/// \brief		Storage for the edge property and connected nodes.
	class edge
	{
	public:
		/// \brief		Source `node`.
		node * source;
		/// \brief		Target `node`.
		node * target;
		/// \brief		Edge property.
		EdgeProperty property;

	protected:
		template <typename ... Args>
		edge(node * source_node, node * target_node, Args && ... edge_property_args);

	friend node_centric;
	};

	/// \brief		 Creates empty graph.
	node_centric() = default;

	/// \brief		 Copy constructor.
	node_centric(const node_centric & other);

	/// \brief		 Move constructor.
	node_centric(node_centric && other) noexcept;

	/// \brief		 Copy assignment.
	node_centric & operator  =(const node_centric & other);

	/// \brief		 Move assignment.
	node_centric & operator  =(node_centric && other) noexcept;

	/// \brief		Deletes all nodes and edges and their properties.
	~node_centric();

	/// \brief		Creates node with property.
	/// \param[in]	node_property_args	Arguments for node property constructor.
	/// \returns	Pointer to the created `node` holding the property. Pointed object is managed
	///				internally and has same lifetime as the owning `node_centric` object.
	template <typename ... Args>
	node * create_node(Args && ... node_property_args);

	/// \brief		Creates edge from source to target node with property.
	/// \param[in]	source_node			Source `node` pointer (returned from `create_node()` call).
	/// \param[in]	target_node			Target `node` pointer (returned from `create_node()` call).
	/// \param[in]	edge_property_args	Arguments for `edge` property constructor.
	/// \returns	Pointer to the created `edge` holding the property and connected `node`s. Pointed object is
	///				managed internally and has same lifetime as the owning `node_centric` object.
	template <typename ... Args>
	edge * create_edge(node * source_node, node * target_node, Args && ... edge_property_args);

	/// \brief		Removes node and all its adjacent edges from the graph.
	///
	/// \par Complexity
	/// *O(e * (o + i) + n)* where *o* - number of outgoing edges, *i* - number of incoming edges, *n* - number of nodes 
	/// in graph, *e* - number of edges in graph).
	void remove_node(node * node_to_remove);

	/// \brief		Removes edge from the graph. 
	///
	/// Nodes are never removed during this operation, even if they remain unconnected.
	///
	/// \par Complexity
	/// *O(o + i + e)* where *o* - number of outgoing edges from source node, *i* - number of incoming edges into 
	/// target node, *e* - number of edges in graph.
	void remove_edge(edge * edge_to_remove);

public:
	/// \brief		Set of graph nodes.
	node_container nodes;
	/// \brief		Set of graph edges.
	edge_container edges;
};

#pragma region node_centric::node implementation:

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
node_centric<NodeProperty, EdgeProperty>::node::node(Args && ... node_property_args) :
	outgoing(),
	incoming(),
	property(std::forward<Args>(node_property_args) ...)
{
}

#pragma endregion

#pragma region node_centric::edge implementation:

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
node_centric<NodeProperty, EdgeProperty>::edge::edge(node * source_node, node * target_node, Args && ... edge_property_args) :
	source(source_node),
	target(target_node),
	property(std::forward<Args>(edge_property_args) ...)
{
}

#pragma endregion

#pragma region node_centric implementation:

template <typename NodeProperty, typename EdgeProperty>
node_centric<NodeProperty, EdgeProperty>::node_centric(const node_centric & other)
{
	// Naive approach:

	// other -> this node morphism
	std::map<const node *, node *> morphism;

	// Recreate all nodes:
	for (const node * other_node_ptr : other.nodes)
	{
		node * this_node_ptr = create_node(other_node_ptr->property);
		morphism.insert(std::make_pair(other_node_ptr, this_node_ptr));
	}

	// Recreate all edges:
	for (const edge * other_edge_ptr : other.edges)
	{
		node * this_source_node = morphism.at(other_edge_ptr->source);
		node * this_target_node = morphism.at(other_edge_ptr->target);
		create_edge(this_source_node, this_target_node, other_edge_ptr->property);
	}
}

template <typename NodeProperty, typename EdgeProperty>
node_centric<NodeProperty, EdgeProperty>::node_centric(node_centric && other) noexcept
{
	std::swap(nodes, other.nodes);
	std::swap(edges, other.edges);
}

template <typename NodeProperty, typename EdgeProperty>
typename node_centric<NodeProperty, EdgeProperty> &
node_centric<NodeProperty, EdgeProperty>::node_centric::operator =(const node_centric & other)
{
	return (*this) = node_centric(other);
}

template <typename NodeProperty, typename EdgeProperty>
typename node_centric<NodeProperty, EdgeProperty> &
node_centric<NodeProperty, EdgeProperty>::node_centric::operator =(node_centric && other) noexcept
{
	std::swap(nodes, other.nodes);
	std::swap(edges, other.edges);

	return (*this);
}

template <typename NodeProperty, typename EdgeProperty>
node_centric<NodeProperty, EdgeProperty>::~node_centric()
{
	for (node * node_ptr : nodes) delete node_ptr;
	for (edge * edge_ptr : edges) delete edge_ptr;
}

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
typename node_centric<NodeProperty, EdgeProperty>::node *
node_centric<NodeProperty, EdgeProperty>::create_node(Args && ... node_property_args)
{
	auto * node_ptr = new node(std::forward<Args>(node_property_args) ...);
	nodes.insert(nodes.end(), node_ptr);
	return node_ptr;
}

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
typename node_centric<NodeProperty, EdgeProperty>::edge *
node_centric<NodeProperty, EdgeProperty>::create_edge(node * source_node, node * target_node, Args && ... edge_property_args)
{
#ifdef _DEBUG
	auto it_source_node = std::find(nodes.begin(), nodes.end(), source_node);
	auto it_target_node = std::find(nodes.begin(), nodes.end(), target_node);

	assert(it_source_node != nodes.end() && "Source node isn't in this graph.");
	assert(it_target_node != nodes.end() && "Target node isn't in this graph.");
#endif

	auto * edge_ptr = new edge(source_node, target_node, std::forward<Args>(edge_property_args) ...);
	edges.insert(edges.end(), edge_ptr);
	source_node->outgoing.push_back(edge_ptr);
	target_node->incoming.push_back(edge_ptr);
	return edge_ptr;
}

template <typename NodeProperty, typename EdgeProperty>
void
node_centric<NodeProperty, EdgeProperty>::remove_node(node * node_to_remove)
{
	// remove all outgoing edges:
	for (edge * edge_ptr : node_to_remove->outgoing)
	{
		// remove edge from vector of incoming edges from target node
		auto & target_node_incoming = edge_ptr->target->incoming;
		target_node_incoming.erase(std::remove(target_node_incoming.begin(), target_node_incoming.end(), edge_ptr));

		// remove from vector of edges
		edges.erase(std::remove(edges.begin(), edges.end(), edge_ptr));

		// delete the edge object
		delete edge_ptr;
	}

	// remove all incoming edges:
	for (edge * edge_ptr : node_to_remove->incoming)
	{
		// remove edge from vector of outgoing edges from source node
		auto & source_node_outgoing = edge_ptr->source->outgoing;
		source_node_outgoing.erase(std::remove(source_node_outgoing.begin(), source_node_outgoing.end(), edge_ptr));

		// remove from vector of edges
		edges.erase(std::remove(edges.begin(), edges.end(), edge_ptr));

		// delete the edge object
		delete edge_ptr;
	}

	// remove from vector of nodes:
	nodes.erase(std::remove(nodes.begin(), nodes.end(), node_to_remove));

	// delete the node object:
	delete node_to_remove;
}

template <typename NodeProperty, typename EdgeProperty>
void
node_centric<NodeProperty, EdgeProperty>::remove_edge(edge * edge_to_remove)
{
	// remove edge from source and target nodes:
	auto & source_outgoing_edges = edge_to_remove->source->outgoing;
	source_outgoing_edges.erase(std::remove(source_outgoing_edges.begin(), source_outgoing_edges.end(), edge_to_remove));
	auto & target_incoming_edges = edge_to_remove->target->incoming;
	target_incoming_edges.erase(std::remove(target_incoming_edges.begin(), target_incoming_edges.end(), edge_to_remove));

	// remove from vector of edges:
	edges.erase(std::remove(edges.begin(), edges.end(), edge_to_remove));

	// delete the edge object:
	delete edge_to_remove;
}

#pragma endregion

} // namespace graph
} // namespace containers
