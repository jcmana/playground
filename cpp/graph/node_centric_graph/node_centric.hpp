#pragma once

#include <vector>
#include <utility>

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
class node_centric_graph
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
	/// \brief		Class holding the node property and sets of outgoing/incoming edges.
	/// \note		Can be constructed only by calling node_centric_graph::create_node(...) method. 
	class node
	{
	public:
		/// \brief		Set of outgoing edges.
		edge_container outgoing;
		/// \brief		Set of incoming edges.		
		edge_container incoming;
		/// \brief		Node property.
		NodeProperty property;

	protected:
		template <typename ... Args>
		node(Args && ... node_property_args);

	private:
		friend node_centric_graph;
	};

	/// \brief		Class holding the edge property and connected nodes.
	/// \note		Can be constructed only by calling node_centric_graph::create_edge(...) method. 
	class edge
	{
	public:
		/// \brief		Edge source node.
		node * source;
		/// \brief		Edge target node.
		node * target;
		/// \brief		Edge property.
		EdgeProperty property;

	protected:
		template <typename ... Args>
		edge(node * source_node, node * target_node, Args && ... edge_property_args);

	private:
		friend node_centric_graph;
	};

	/// \brief		Creates node with property.
	/// \param[in]	...				Arguments for node property constructor.
	/// \returns	Pointer to the created node holding the property. Pointed object is managed
	///				internally and has guaranteed same lifetime as the owning node_centric_graph object.
	template <typename ... Args>
	node * create_node(Args && ... node_property_args);

	/// \brief		Creates edge from source to target node with property.
	/// \param[in]	source_node		Edge source node (pointer returned from create_node(...) call).
	/// \param[in]	target_node		Edge target node (pointer returned from create_node(...) call).
	/// \param[in]	...				Arguments for edge property constructor.
	/// \returns	Pointer to the created edge holding the property and connected nodes. Pointed object is
	///				managed internally and has guaranteed lifetime same as the owning `node_centric_graph` object.
	template <typename ... Args>
	edge * create_edge(node * source_node, node * target_node, Args && ... edge_property_args);

	/// \brief		Removes node and all its adjacent edges from the graph.
	void remove_node(node * node_to_remove);

	/// \brief		Removes edge from the graph.
	void remove_edge(edge * edge_to_remove);

	/// \brief		Deletes all nodes and edges and their properties.
	~node_centric_graph();

public:
	/// \brief		Set of graph nodes.
	node_container nodes;
	/// \brief		Set of graph edges.
	edge_container edges;
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
	nodes.insert(nodes.end(), node_ptr);
	return node_ptr;
}

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
typename node_centric_graph<NodeProperty, EdgeProperty>::edge *
node_centric_graph<NodeProperty, EdgeProperty>::create_edge(node * source_node, node * target_node, Args && ... edge_property_args)
{
	auto * edge_ptr = new edge(source_node, target_node, std::forward<Args>(edge_property_args) ...);
	edges.insert(edges.end(), edge_ptr);
	source_node->outgoing.push_back(edge_ptr);
	target_node->incoming.push_back(edge_ptr);
	return edge_ptr;
}

template <typename NodeProperty, typename EdgeProperty>
void
node_centric_graph<NodeProperty, EdgeProperty>::remove_node(node * node_to_remove)
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
node_centric_graph<NodeProperty, EdgeProperty>::remove_edge(edge * edge_to_remove)
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

template <typename NodeProperty, typename EdgeProperty>
node_centric_graph<NodeProperty, EdgeProperty>::~node_centric_graph()
{
	for (node * node_ptr : nodes) delete node_ptr;
	for (edge * edge_ptr : edges) delete edge_ptr;
}
