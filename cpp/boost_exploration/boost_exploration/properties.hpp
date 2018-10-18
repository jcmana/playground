#pragma once

#include <boost/graph/adjacency_list.hpp>

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
		typedef boost::edge_property_tag type;
	};

	using edge_property = boost::property<edge_type_t, edge_type_e>;

	using graph = boost::adjacency_list<
		boost::setS,
		boost::setS,
		boost::directedS,
		boost::no_property,
		edge_property,
		boost::no_property>;

	graph g;

	boost::property_map<graph, edge_type_t>::type edge_types = boost::get(edge_type_t(), g);
}