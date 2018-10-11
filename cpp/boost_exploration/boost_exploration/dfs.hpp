#pragma once

#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/irange.hpp>

const std::string dfs_names[]
{
	"struct_1", "struct_2", "struct_3",
	"ref_0", "ref_1", "ref_2", "ref_3",
	"el_1", "el_2", "el_3", "el_4", "el_5", "el_6"
};

class DfsGraphVisitor :
	public boost::default_dfs_visitor
{
public:
	template<typename Vertex, typename Graph>
	void discover_vertex(Vertex u, const Graph & g) const
	{
		std::cout << "DfsGraphVisitor::discover_vertex(): " << dfs_names[u] << std::endl;
	}
};

void test_dfs()
{
	// Set up the vertex IDs and names
	enum VertexLabel
	{
		struct_1, struct_2, struct_3,
		ref_0, ref_1, ref_2, ref_3,
		el_1, el_2, el_3, el_4, el_5, el_6
	};

	using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;
	using Edge = std::pair<int, int>;

	// Specify the edges in the graph
	std::vector<Edge> edges
	{
		{ ref_0, struct_1 },

		{ struct_1, ref_1 },
		{ struct_1, ref_2 },
		{ struct_1, el_1 },

		{ struct_2, el_2 },
		{ struct_2, el_3 },

		{ struct_3, el_4 },
		{ struct_3, el_5 },
		{ struct_3, ref_3 },

		{ ref_1, struct_2 },
		{ ref_2, struct_3 },
		{ ref_3, struct_2 }
	};

	// Create the graph object
	Graph g(edges.begin(), edges.end(), edges.size());

	// Iterated over graph vertices
	{
		Graph::vertex_iterator it;
		Graph::vertex_iterator it_end;
	
		for (boost::tie(it, it_end) = boost::vertices(g); it != it_end; it++)
		{
			auto v = boost::vertex(*it, g);
			std::cout << "vertex = " << dfs_names[v] << std::endl;
		}
	}

	// Iterate over graph edges
	{
		Graph::edge_iterator it;
		Graph::edge_iterator it_end;

		for (boost::tie(it, it_end) = boost::edges(g); it != it_end; it++)
		{
			auto edge_descriptor = *it;

			std::cout << "edge = " << dfs_names[edge_descriptor.m_source] << " -> " << dfs_names[edge_descriptor.m_target] << std::endl;
		}
	}

	// Run DFS search
	DfsGraphVisitor visitor;
	boost::depth_first_search(g, boost::visitor(visitor));

	std::getchar();
}
