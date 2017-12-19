
/* New Digraph API */

#ifndef LEXBUILD_DIGRAPH_HPP
#define LEXBUILD_DIGRAPH_HPP

#include "Graph/Edge.hpp"

#include <queue>
#include <unordered_map>
#include <unordered_set>

template <class T, class K>
class Digraph
{
public:
	/**
	 * Construct a new vertex for the digraph
	 * @return the constructed vertex id
	 */
	int make_vertex();

	/* TODO: (Possible fix)
	 * Consider storing incoming(vertex).
	 */

	/**
	 * Remove a vertex from the digraph.
	 *
	 *   Note: This is an *expensive* operation to conduct
	 *   because outdegree(vertex) + indegree(vertex) links
	 *   need to be removed.
	 *
	 *   The main problem is computing the incoming edges of
	 *   vertex in this digraph implementation, which requires
	 *   going through every edge in the graph.
	 *
	 *   Use with caution.
	 *
	 * @param vertex - vertex to remove
	 * @return true, if the vertex was successfully freed
	 */
	bool remove_vertex(int vertex);

	/**
	 * Add an edge to the graph
	 * @param src - source vertex
	 * @param dest - destination vertex
	 * @param weight - edge weight
	 * @param data - extra data
	 * @return true if the edge was added
	 */
	bool add_edge(int src, int dest, T weight, K data);

	/**
	 * Remove edge from the graph
	 * @param src - source vertex
	 * @param dest - destination vertex
	 * @return true if the edge was removed
	 */
	bool remove_edge(int src, int dest);

	/**
	 * Vertex count in the graph
	 * @return vertex count
	 */
	int vertices();

	/**
	 * Edge count in the graph
	 * @return edge count
	 */
	int edges();

private:
	/* New vertex generator */
	int v_next = 0;

	/* Current vertex count */
	int v_count = 0;

	/* Current edge count */
	int e_count = 0;

	/* Free nodes (to be re-alloc'd) */
	std::queue<int> avail{};

	/* Adjacency list for the graph */
	/*  NOTE: parallel edges are *not* supported */
	std::unordered_map<int, std::unordered_set<Edge<T, K>>> adj_list{};

	/* Is the vertex valid? */
	bool valid(int vertex);
};

template <class T, class K>
int Digraph<T, K>::make_vertex()
{
	if(!avail.empty())
	{
		int vertex = avail.front();

		avail.pop();

		v_count++;
		return vertex;
	}

	int vertex = v_next;
	v_next++;

	v_count++;
	return vertex;
}

template <class T, class K>
bool Digraph<T, K>::remove_vertex(int vertex)
{
	if(!valid(vertex))
		return false;

	/* TODO link removal + avail adding */

	v_count--;
	return true;
}

template<class T, class K>
bool Digraph<T, K>::add_edge(int src, int dest, T weight, K data)
{
	if(!valid(src) || !valid(dest))
		return false;

	if(adj_list[src].insert(Edge(src, dest, weight, data)).second)
	{
		e_count++;
		return true;
	}

	return false;
}

template<class T, class K>
bool Digraph<T, K>::remove_edge(int src, int dest)
{
	if(!valid(src) || !valid(dest))
		return false;

	auto& set = adj_list[src];

	for(auto itr = set.begin(); itr != set.end(); itr++)
	{
		Edge edge = *itr;

		if(edge.get_src() == src && edge.get_dest() == dest)
		{
			set.erase(itr);
			e_count--;

			return true;
		}
	}

	return false;
}

template <class T, class K>
bool Digraph<T, K>::valid(int vertex)
{
	return vertex >= v_next;
}

template<class T, class K>
int Digraph<T, K>::vertices()
{
	return v_count;
}

template<class T, class K>
int Digraph<T, K>::edges()
{
	return e_count;
}



#endif
