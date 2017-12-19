
/* New Digraph API */

#ifndef LEXBUILD_DIGRAPH_HPP
#define LEXBUILD_DIGRAPH_HPP

#include <queue>

template <class T>
class Digraph
{
public:
	/**
	 * Construct a new vertex for the digraph
	 * @return the constructed vertex id
	 */
	int make_vertex();

	/**
	 * Remove a vertex from the digraph.
	 *
	 *   Note: This is an *expensive* operation to conduct
	 *   because outdegree(vertex) + indegree(vertex) links
	 *   need to be removed.
	 *
	 *   The main problem is computing the incoming edges of
	 *   vertex in this digraph implementation takes O(V * E) time.
	 *
	 *   Use with caution.
	 *
	 * @param vertex - vertex to remove
	 * @return true, if the vertex was successfully freed
	 */
	bool remove_vertex(int vertex);

private:
	/* New vertex count */
	int v_count = 0;

	/* Freed nodes (to be re-alloc'd) */
	std::queue<int> avail;
};

template <class T>
int Digraph<T>::make_vertex()
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

template <class T>
bool Digraph<T>::remove_vertex(int vertex)
{
	/* TODO */
}

#endif
