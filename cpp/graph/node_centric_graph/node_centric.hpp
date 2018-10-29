#pragma once

#include <set>
#include <utility>

/// \brief		Directed graph with node and edges properties.
///
/// \par Properties:
///		- directed edges
///		- fast incoming/outgoing edges access
///		- custom node property
///		- custom edge property
///
/// Graph itself doesn't support any typical traverses or algorithms, therefore serves exclusivelly
/// as underlaying structure holding the information about nodes and edges and allows basic iteration
/// over those.
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
struct node_centric_graph
{
	class node;
	class edge;

	/// \brief		Class holding the edge property and informations about connected nodes.
	/// \note		Can be constructed only by calling node_centric_graph::create_edge(...) method. 
	class node
	{
	protected:
		template <typename ... Args>
		node(Args && ... node_property_args);

	public:
		/// \brief		Set of outgoing edges.
		std::set<edge *> outgoing;
		/// \brief		Set of incoming edges.		
		std::set<edge *> incoming;
		/// \brief		Node property.
		NodeProperty property;

	private:
		friend node_centric_graph;
	};

	/// \brief		Class holding the edge property.
	/// \note		Can be constructed only by calling node_centric_graph::create_edge(...) method. 
	class edge
	{
	protected:
		template <typename ... Args>
		edge(node * source_node, node * target_node, Args && ... edge_property_args);

	public:
		/// \brief		Edge source node.
		node * source;
		/// \brief		Edge target node.
		node * target;
		/// \brief		Edge property.
		EdgeProperty property;

	private:
		friend node_centric_graph;
	};

	/// \brief		Creates node with property.
	/// \param[in]	...				Arguments for property constructor.
	/// \returns	Pointer to the created node holding the property.
	template <typename ... Args>
	node * create_node(Args && ... node_property_args);

	/// \brief		Creates edge from source to target node with property.
	/// \param[in]	source_node		Edge source node (returned pointer from create_node(...) call).
	/// \param[in]	target_node		Edge target node (returned pointer from create_node(...) call).
	/// \param[in]	...				Arguments for property constructor.
	/// \returns	Pointer to the created holding holding the property and connected nodes.
	template <typename ... Args>
	edge * create_edge(node * source_node, node * target_node, Args && ... edge_property_args);

	/// \brief		Deletes all nodes and edges and their properties.
	~node_centric_graph();

	/// \brief		Set of graph nodes.
	std::set<node *> nodes;
	/// \brief		Set of graph edges.
	std::set<edge *> edges;
};

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
node_centric_graph<NodeProperty, EdgeProperty>::node::node(Args && ... node_property_args) :
	outgoing(),
	incoming(),
	property(std::forward<Args>(node_property_args) ...)
{
}

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
node_centric_graph<NodeProperty, EdgeProperty>::edge::edge(node * source_node, node * target_node, Args && ... edge_property_args) :
	source(source_node),
	target(target_node),
	property(std::forward<Args>(edge_property_args) ...)
{
}

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
typename node_centric_graph<NodeProperty, EdgeProperty>::node *
node_centric_graph<NodeProperty, EdgeProperty>::create_node(Args && ... node_property_args)
{
	auto * node_ptr = new node(std::forward<Args>(node_property_args) ...);
	nodes.insert(node_ptr);
	return node_ptr;
}

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
typename node_centric_graph<NodeProperty, EdgeProperty>::edge *
node_centric_graph<NodeProperty, EdgeProperty>::create_edge(node * source_node, node * target_node, Args && ... edge_property_args)
{
	auto * edge_ptr = new edge(source_node, target_node, std::forward<Args>(edge_property_args) ...);
	edges.insert(edge_ptr);
	source_node->outgoing.insert(edge_ptr);
	target_node->incoming.insert(edge_ptr);
	return edge_ptr;
}

template <typename NodeProperty, typename EdgeProperty>
node_centric_graph<NodeProperty, EdgeProperty>::~node_centric_graph()
{
	for (auto & it_node : nodes) delete it_node;
	for (auto & it_edge : edges) delete it_edge;
}
