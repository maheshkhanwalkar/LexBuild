
/* New Digraph API */

#ifndef LEXBUILD_DIGRAPH_HPP
#define LEXBUILD_DIGRAPH_HPP

#include "Graph/Edge.hpp"

#include <queue>
#include <unordered_map>
#include <unordered_set>

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
	 * Note: This operation needs to remove:
	 *    outdegree(vertex) + indegree(vertex) links
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
	 * @return true if the edge was added
	 */
	bool add_edge(int src, int dest, T weight);

	/**
	 * Remove edge from the graph
	 * @param src - source vertex
	 * @param dest - destination vertex
	 * @return true if the edge was removed
	 */
	bool remove_edge(int src, int dest);

	/**
	 * Check if there is an edge
	 * @param src - source vertex
	 * @param dest - destination vertex
	 * @return true if the edge exists, false otherwise
	 */
	bool has_edge(int src, int dest);

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

	/**
	 * Outgoing edges of a vertex
	 * @param src - vertex to examine
	 * @return set of outgoing edges from src
	 */
	std::unordered_set<Edge<T>, EdgeHash<T>> outgoing(int src);

	/**
	 * Incoming edges of a vertex
	 * @param dest - vertex to examine
	 * @return set of incoming edges to dest
	 */
	std::unordered_set<Edge<T>, EdgeHash<T>> incoming(int dest);

	/**
	 * In-degree of a vertex
	 * @param vertex - vertex to consider
	 * @return -1 if vertex is invalid, else in-degree of vertex
	 */
	int indegree(int vertex);

	/**
	 * Out-degree of a vertex
	 * @param vertex - vertex to consider
	 * @return -1 if vertex is invalid, else out-degree of vertex
	 */
	int outdegree(int vertex);

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
	std::unordered_map<int, std::unordered_set<Edge<T>, EdgeHash<T>>> adj_map{};

	/* Incoming edge map */
	std::unordered_map<int, std::unordered_set<Edge<T>, EdgeHash<T>>> in_map{};

	/* Hash function for Edge<T> */
	struct edge_hash
	{
		size_t operator() (const Edge<T>& edge) const
		{
			std::hash<int> h_func;

			int src = edge.get_src();
			int dest = edge.get_dest();

			return std::__hash_combine(h_func(src), h_func(dest));
		}
	};

protected:
	/* Is the vertex valid? */
	bool valid(int vertex);
};

template <class T>
int Digraph<T>::make_vertex()
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

template <class T>
bool Digraph<T>::remove_vertex(int vertex)
{
	if(!valid(vertex))
		return false;

	size_t out_degree = adj_map[vertex].size();
	adj_map[vertex].clear();

	e_count -= out_degree;

	for(Edge<T> e : in_map[vertex])
		remove_edge(e.get_src(), e.get_dest());

	v_count--;
	return true;
}

template<class T>
bool Digraph<T>::add_edge(int src, int dest, T weight)
{
	if(!valid(src) || !valid(dest))
		return false;

	Edge<T> edge(src, dest, weight);

	if(adj_map[src].insert(edge).second)
	{
		e_count++;
		in_map[dest].insert(edge);

		return true;
	}

	return false;
}

template<class T>
bool Digraph<T>::remove_edge(int src, int dest)
{
	if(!valid(src) || !valid(dest))
		return false;

	auto& src_set = adj_map[src];
	auto& dest_set = in_map[dest];

	bool found = false;

	for(auto itr = src_set.begin(); itr != src_set.end(); itr++)
	{
		Edge<T> edge = *itr;

		if(edge.get_src() == src && edge.get_dest() == dest)
		{
			src_set.erase(itr);
			e_count--;

			found = true;
			break;
		}
	}

	if(!found)
		return false;

	for(auto itr = dest_set.begin(); itr != dest_set.end(); itr++)
	{
		Edge<T> edge = *itr;

		if(edge.get_src() == src && edge.get_dest() == dest)
		{
			dest_set.erase(itr);
			e_count--;

			return true;
		}
	}

	return false;
}

template<class T>
bool Digraph<T>::has_edge(int src, int dest)
{
	if(!valid(src) || !valid(dest))
		return false;

	for(Edge<T> e : adj_map[src])
	{
		if(e.get_dest() == dest)
			return true;
	}

	return false;
}

template <class T>
bool Digraph<T>::valid(int vertex)
{
	return vertex < v_next;
}

template<class T>
int Digraph<T>::vertices()
{
	return v_count;
}

template<class T>
int Digraph<T>::edges()
{
	return e_count;
}

template<class T>
std::unordered_set<Edge<T>, EdgeHash<T>> Digraph<T>::outgoing(int src)
{
	if(!valid(src))
		return std::unordered_set<Edge<T>, EdgeHash<T>>();

	return adj_map[src];
}

template<class T>
std::unordered_set<Edge<T>, EdgeHash<T>> Digraph<T>::incoming(int dest)
{
	if(!valid(dest))
		return std::unordered_set<Edge<T>, EdgeHash<T>>();

	return in_map[dest];
}

template<class T>
int Digraph<T>::indegree(int vertex)
{
	if(!valid(vertex))
		return -1;

	return static_cast<int>(in_map[vertex].size());
}

template<class T>
int Digraph<T>::outdegree(int vertex)
{
	if(!valid(vertex))
		return -1;

	return static_cast<int>(adj_map[vertex].size());
}

#endif
