#pragma once

#include <map>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>

void test_properties()
{
	enum class edge_type_e
	{
		typeless,
		up, down,
		above, below
	};

	struct edge_type_t
	{
		using type = boost::edge_property_tag;
	};

	using node_property = boost::no_property;
	using edge_property = boost::property<edge_type_t, edge_type_e>;

	edge_property p(edge_type_e::down);

	using graph = boost::adjacency_list<
		boost::setS,			// edge storage
		boost::setS,			// vertex storage
		boost::directedS,		// edge direction
		node_property,			// vertex property
		edge_property,			// edge property
		boost::no_property>;	// graph property

	graph g;

	auto vertex_1 = boost::add_vertex(g);
	auto vertex_2 = boost::add_vertex(g);

	auto edge_1_2 = boost::add_edge(vertex_1, vertex_2, g);

	boost::put(boost::vertex_name_t(), g, edge_1_2, p);
	boost::property_map<graph, edge_type_t>::type et = boost::get(edge_type_t(), g);



	return;
}