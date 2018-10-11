#pragma once

#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/irange.hpp>

const std::string bfs_names[]
{
	"struct_1", "struct_2", "struct_3",
	"ref_0", "ref_1", "ref_2", "ref_3",
	"el_1", "el_2", "el_3", "el_4", "el_5", "el_6"
};

class BfsGraphVisitor :
	public boost::default_bfs_visitor
{
public:
	template<typename Vertex, typename Graph>
	void discover_vertex(Vertex u, const Graph & g) const
	{
		//std::cout << "BfsGraphVisitor::discover_vertex(): " << names[u] << std::endl;
	}

	template<typename Vertex, typename Graph>
	void examine_vertex(Vertex u, const Graph & g) const
	{
		std::cout << "BfsGraphVisitor::examine_vertex(): " << bfs_names[u] << std::endl;
	}
};

void test_bfs()
{
	// Set up the vertex IDs and names
	enum VertexLabel
	{
		struct_1, struct_2, struct_3,
		ref_0, ref_1, ref_2, ref_3,
		el_1, el_2, el_3, el_4, el_5, el_6
	};

	using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS>;
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
			std::cout << "vertex = " << bfs_names[v] << std::endl;
		}
	}

	// Iterate over graph edges
	{
		Graph::edge_iterator it;
		Graph::edge_iterator it_end;

		for (boost::tie(it, it_end) = boost::edges(g); it != it_end; it++)
		{
			auto edge = boost::edge(it->m_source, it->m_target, g);
			auto edge_descriptor = edge.first;
			auto edge_is_valid = edge.second;
			if (edge_is_valid)
			{
				std::cout << "edge = " << bfs_names[edge_descriptor.m_source] << " -> " << bfs_names[edge_descriptor.m_target] << std::endl;
			}
		}
	}

	// Run BFS search
	BfsGraphVisitor visitor;
	boost::breadth_first_search(g, boost::vertex(ref_0, g), boost::visitor(visitor));

	std::getchar();
}
