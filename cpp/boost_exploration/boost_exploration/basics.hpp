#pragma once

#include <array>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/directed_graph.hpp>

inline void test_basics()
{
	using Graph = boost::adjacency_list<
		boost::vecS,
		boost::vecS,
		boost::directedS>;
	using Edge = std::pair<int, int>;
	using IndexMap = boost::property_map<Graph, boost::vertex_index_t>::type;
	using VertexIterator = boost::graph_traits<Graph>::vertex_iterator;

	// make convenient labels for the vertices
	enum { A, B, C, D, E, N };
	const int num_vertices = N;
	const char * name = "ABCDE";

	// writing out the edges in the graph
	Edge edge_array[] = { Edge(A,B), Edge(A,D), Edge(C,A), Edge(D,C), Edge(C,E), Edge(B,D), Edge(D,E) };
	const int num_edges = (sizeof(edge_array) / sizeof(edge_array[0]));

	// create a graph object with all edges
	Graph g(edge_array, edge_array + sizeof(edge_array) / sizeof(Edge), num_vertices);

	// get the property map for vertex indices
	IndexMap index = get(boost::vertex_index, g);

	// accessing vertices:
	{
		std::cout << "vertices(g) = ";

		std::pair<VertexIterator, VertexIterator> vp;
		for (vp = vertices(g); vp.first != vp.second; ++vp.first)
		{
			std::cout << index[*vp.first] << " ";
		}
		std::cout << std::endl;
	}

	// accessing edges:
	{
		std::cout << "edges(g) = ";
		boost::graph_traits<Graph>::edge_iterator ei;
		boost::graph_traits<Graph>::edge_iterator ei_end;
		for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
		{
			std::cout << "(" << index[source(*ei, g)];
			std::cout << "," << index[target(*ei, g)];
			std::cout << ") ";
		}
		std::cout << std::endl;
	}

	std::getchar();
}