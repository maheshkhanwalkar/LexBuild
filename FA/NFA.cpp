
#include "NFA.hpp"
#include <queue>

NFA::NFA(int s_state)
{
	this->s_state = s_state;
}

std::unique_ptr<DFA> NFA::transform()
{
	std::unordered_map<std::unordered_set<int>, int, set_hash> name_conv;
	std::unordered_set<int> start_set = expand(s_state);

	std::unique_ptr<DFA> result = std::make_unique<DFA>(0);

	for (auto vertex : start_set)
	{
		/* Mark as accept, if an accept state is a part of the set */
		if (accept.find(vertex) != accept.end())
		{
			result->set_accept(0);
			break;
		}
	}


	/* Perform e-transition expansion on the rest of the graph */
	int count = 0;

	std::queue<std::unordered_set<int>> ex_queue;
	ex_queue.push(start_set);

	name_conv.insert({start_set, count});
	count++;

	while(!ex_queue.empty())
	{
		std::unordered_set<int> current = ex_queue.front();
		ex_queue.pop();

		auto edge_map = unique_edges(current);

		for(const auto& item : *edge_map)
		{
			std::unordered_set<int> total;

			/* Merge all the expanded states */
			for(int dest : item.second)
			{
				std::unordered_set<int> exp_state = expand(dest);
				std::copy(exp_state.begin(), exp_state.end(), std::inserter(total, total.end()));
			}

			/* New state ? */
			if(name_conv.find(total) == name_conv.end())
			{
				name_conv.insert({total, count});
				count++;

				ex_queue.push(total);
			}

			/* Add edge to DFA */
			result->add_edge(name_conv[current], name_conv[total], item.first);

			for (int vertex : total)
			{
				/* Determine if the expanded state is an accept state */
				if(accept.find(vertex) != accept.end())
				{
					result->set_accept(name_conv[total]);
					break;
				}
			}
		}
	}

	return result;
}

std::unique_ptr<std::unordered_map<char, std::unordered_set<int>>>
	NFA::unique_edges(const std::unordered_set<int>& node)
{
	auto map = std::make_unique<std::unordered_map<char, std::unordered_set<int>>>();

	for (auto sub : node)
	{
		auto edges = adj_list[sub];

		for(auto edge : edges)
		{
			/* Ignore epsilons */
			if(edge.second.second == Edge_Type::EPSILON)
				continue;

			(*map)[edge.second.first].insert(edge.first);
		}
	}

	return map;
}

std::unordered_set<int> NFA::expand(int state)
{
	std::unordered_set<int> result;
	std::unordered_set<int> visited;

	std::queue<int> queue;
	queue.push(state);

	while(!queue.empty())
	{
		int current = queue.front();
		queue.pop();

		visited.insert(current);
		result.insert(current);

		const auto& edges = adj_list[current];

		for(const auto& edge : edges)
		{
			/* Keep adding what we can reach by e-transitions */
			if(edge.second.second == Edge_Type::EPSILON && visited.find(edge.first) == visited.end())
			{
				visited.insert(edge.first);
				queue.push(edge.first);
			}
		}
	}

	return result;
}

void NFA::set_accept(int state)
{
	accept.insert(state);
}

bool NFA::is_accept(int state)
{
	return accept.find(state) != accept.end();
}

