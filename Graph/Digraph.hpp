
/* New Digraph API */

#ifndef LEXBUILD_DIGRAPH_HPP
#define LEXBUILD_DIGRAPH_HPP

#include "Graph/Edge.hpp"
#include <queue>

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

private:
	/* New vertex count */
	int v_count = 0;

	/* Free nodes (to be re-alloc'd) */
	std::queue<int> avail;

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
		return vertex;
	}

	int vertex = v_count;
	v_count++;

	return vertex;
}

template <class T, class K>
bool Digraph<T, K>::remove_vertex(int vertex)
{
	/* TODO */
}

template<class T, class K>
bool Digraph<T, K>::add_edge(int src, int dest, T weight, K data)
{
	if(!valid(src) || !valid(dest))
		return false;

	/* TODO */

	return true;
}

template <class T, class K>
bool Digraph<T, K>::valid(int vertex)
{
	return vertex >= v_count;
}


#endif
