
#ifndef LEXBUILD_WEIGHTEDDIGRAPH_HPP
#define LEXBUILD_WEIGHTEDDIGRAPH_HPP

#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

template <class T>
class WeightedDigraph
{
public:

	/**
	 * Add an edge: src -> dest
	 * @param src - source vertex
	 * @param dest - destination vertex
	 * @param weight - edge weight
	 */
	virtual void add_edge(int src, int dest, T weight)
	{
		adj_list[src].push_back(std::make_pair(dest, weight));
	}

	/**
	 * Remove src -> dest edge
	 * @param src - source vertex
	 * @param dest - destination vertex
	 * @return if the edge was removed (false if edge doesn't exist)
	 */
	bool remove_edge(int src, int dest)
	{
		for (auto itr = adj_list[src].begin(); itr != adj_list[src].end(); itr++)
		{
			if (itr->first == dest)
			{
				adj_list[src].erase(itr);
				return true;
			}
		}

		return false;
	}

	/**
	 * Check if there is an edge: src -> dest.
	 * @param src - source vertex
	 * @param dest - destination vertex
	 * @return if src -> dest edge exists
	 */
	bool has_edge(int src, int dest)
	{
		for (auto itr = adj_list[src].begin(); itr != adj_list[src].end(); itr++)
		{
			if (itr->first == dest)
				return true;
		}

		return false;
	}

	/**
	 * Weight(s) of the edge: src -> dest
	 * @param src - source vertex
	 * @param dest - destination vertex
	 * @return weight(s) of the connecting edge(s)
	 */
	std::unique_ptr<std::vector<T>> get_weight(int src, int dest)
	{
		auto weights = std::make_unique<std::vector<T>>();

		for (auto itr = adj_list[src].begin(); itr != adj_list[src].end(); itr++)
		{
			if (itr->first == dest)
				weights->push_back(itr->second);
		}

		if(weights->size() == 0)
			return nullptr;

		return weights;
	}

	/**
	 * Edges of a vertex
	 * @param src - source vertex
	 * @return - destination vertices and the edge weights
	 */
	std::vector<std::pair<int, T>> edges(int src)
	{
		return adj_list[src];
	}

protected:
	std::unordered_map<int, std::vector<std::pair<int, T>>> adj_list;
};


#endif
