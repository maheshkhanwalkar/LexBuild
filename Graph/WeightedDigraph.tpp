
#ifndef LEXBUILD_WEIGHTEDDIGRAPH_HPP
#define LEXBUILD_WEIGHTEDDIGRAPH_HPP

#include <unordered_map>
#include <vector>
#include <memory>


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
	 * Weight of the edge: src -> dest
	 * @param src - source vertex
	 * @param dest - destination vertex
	 * @return weight of the connecting edge
	 */
	std::shared_ptr<T> get_weight(int src, int dest)
	{
		for (auto itr = adj_list[src].begin(); itr != adj_list[src].end(); itr++)
		{
			if (itr->first == dest)
				return std::make_shared<T>(itr->second);
		}

		return nullptr;
	}

protected:
	std::unordered_map<int, std::vector<std::pair<int, T>>> adj_list;
};


#endif
