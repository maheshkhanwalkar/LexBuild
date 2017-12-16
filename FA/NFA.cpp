
#include "NFA.hpp"

#include <queue>

NFA::NFA(int s_state)
{
	this->s_state = s_state;
}

std::shared_ptr<DFA> NFA::transform()
{
	std::unordered_map<std::unordered_set<int>, int, set_hash> name_conv;

	std::unordered_set<int> start_set;
	std::queue<int> ep_queue;

	ep_queue.push(s_state);

	/* Determine start state */
	while(!ep_queue.empty())
	{
		int current = ep_queue.front();
		ep_queue.pop();

		start_set.insert(current);

		std::vector<std::pair<int, std::pair<char, Edge_Type>>> edges = adj_list[current];

		for(auto edge : edges)
		{
			/* Keep adding what we can reach by e-transitions */
			if(edge.second.second == Edge_Type::EPSILON)
			{
				if(start_set.find(edge.first) == start_set.end())
					ep_queue.push(edge.first);
			}
		}
	}

	int count = 0;
	name_conv.insert({start_set, count});

	std::shared_ptr<DFA> result = std::make_shared<DFA>(DFA(count));

	for (auto vertex : start_set)
	{
		/* Mark as accept, if an accept state is a part of the set */
		if (accept.find(vertex) != accept.end())
		{
			result->set_accept(count);
			break;
		}
	}

	count++;

	/* Perform e-transition expansion on the rest of the graph */
	/* TODO */

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
