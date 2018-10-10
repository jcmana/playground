//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/irange.hpp>

#include <iostream>

using namespace boost;

template<typename TimeMap>
class bfs_time_visitor :
	public default_bfs_visitor
{
private:
	using T = typename property_traits < TimeMap >::value_type;

public:
	bfs_time_visitor(TimeMap tmap, T & t) :
		m_timemap(tmap),
		m_time(t)
	{
	}

	template<typename Vertex, typename Graph>
	void discover_vertex(Vertex u, const Graph & g) const
	{
		put(m_timemap, u, m_time++);
	}

	TimeMap m_timemap;
	T & m_time;
};

using Graph = adjacency_list<vecS, vecS, undirectedS>;
using Edge = std::pair<int, int>;
using Size = graph_traits <Graph>::vertices_size_type;

using dtime_pm_type = iterator_property_map<
	std::vector<Size>::iterator,
	property_map<Graph, vertex_index_t>::const_type>;

int main()
{
	// Set up the vertex IDs and names
	enum { r, s, t, u, v, w, x, y, N };
	const char *name = "rstuvwxy";
	
	// Specify the edges in the graph
	Edge edge_array[] = { Edge(r, s), Edge(r, v), Edge(s, w), Edge(w, r), Edge(w, t), Edge(w, x), Edge(x, t), Edge(t, u), Edge(x, y), Edge(u, y) };
	const int n_edges = sizeof(edge_array) / sizeof(Edge);

	// Create the graph object
	Graph g(edge_array, edge_array + n_edges, Size(N));

	// Vector to hold the discover time property for each vertex
	std::vector<Size> dtime(num_vertices(g));

	dtime_pm_type dtime_pm(dtime.begin(), get(vertex_index, g));

	Size time = 0;

	bfs_time_visitor<dtime_pm_type> vis(dtime_pm, time);
	breadth_first_search(g, vertex(s, g), visitor(vis));

	// Use std::sort to order the vertices by their discover time
	std::vector<graph_traits<Graph>::vertices_size_type> discover_order(N);
	integer_range<int>range(0, N);
	
	std::copy(range.begin(), range.end(), discover_order.begin());
	std::sort(discover_order.begin(), discover_order.end(), indirect_cmp<dtime_pm_type, std::less<Size>>(dtime_pm));

	// Print discovered verteces in order
	std::cout << "order of discovery: ";
	for (int i = 0; i < N; ++i)
	{
		std::cout << name[discover_order[i]] << " ";
	}
	std::cout << std::endl;

	std::getchar(); return EXIT_SUCCESS;
}