
#include "NFA.hpp"
#include <queue>

NFA::NFA()
{
	this->s_state = make_vertex();
}

std::unique_ptr<DFA> NFA::transform()
{
	std::unordered_map<std::unordered_set<int>, int, set_hash> name_conv;
	std::unordered_set<int> start_set = expand(s_state);

	std::unique_ptr<DFA> result = std::make_unique<DFA>();

	int n_start = make_vertex();
	result->set_start(n_start);

	for (auto vertex : start_set)
	{
		/* Mark as accept, if an accept state is a part of the set */
		if (accept.find(vertex) != accept.end())
		{
			result->add_accept(n_start);
			break;
		}
	}

	/* Perform e-transition expansion on the rest of the graph */

	std::queue<std::unordered_set<int>> ex_queue;
	ex_queue.push(start_set);

	name_conv.insert({start_set, n_start});

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
				int vertex = make_vertex();

				name_conv.insert({total, vertex});
				ex_queue.push(total);
			}

			/* Add edge to DFA */
			result->add_edge(name_conv[current], name_conv[total], item.first);

			for (int vertex : total)
			{
				/* Determine if the expanded state is an accept state */
				if(accept.find(vertex) != accept.end())
				{
					result->add_accept(name_conv[total]);
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
		auto edges = outgoing(sub);

		for(auto edge : edges)
		{
			/* Ignore epsilons */
			NFAWeight weight = edge.get_weight();

			if(weight.get_type() == WeightType::EPSILON)
				continue;

			(*map)[weight.get_weight()].insert(edge.get_dest());
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

		const auto& edges = outgoing(current);

		for(const auto& edge : edges)
		{
			/* Keep adding what we can reach by e-transitions */
			NFAWeight weight = edge.get_weight();

			if(weight.get_type() == WeightType::EPSILON && visited.find(edge.get_dest()) == visited.end())
			{
				visited.insert(edge.get_dest());
				queue.push(edge.get_dest());
			}
		}
	}

	return result;
}

bool NFA::merge_at(NFA& other, int where)
{
	std::unordered_map<int, int> rename;

	if(!valid(where))
		return false;

	int p_start = make_vertex();
	rename.insert({other.s_state, p_start});

	/* Merge 'other' start state to 'where' */
	add_edge(where, p_start, NFAWeight(WeightType::EPSILON, 0));

	/* Merge the result of other */

	std::queue<int> process;
	std::unordered_set<int> visited;

	process.push(other.s_state);

	while(!process.empty())
	{
		int current = process.front();
		process.pop();

		visited.insert(current);

		/* Fix accept states */
		if(other.is_accept(current))
			add_accept(rename[current]);

		const auto& edges = other.outgoing(current);

		for(const auto& edge : edges)
		{
			if(rename.find(edge.get_dest()) == rename.end())
			{
				int vertex = make_vertex();
				rename.insert({edge.get_dest(), vertex});
			}

			add_edge(rename[current], rename[edge.get_dest()], edge.get_weight());

			if (visited.find(edge.get_dest()) == visited.end())
				process.push(edge.get_dest());
		}
	}

	return true;
}

void NFA::merge(NFA& other)
{
	std::unordered_map<int, int> rename;
	consolidate();

	int p_start = make_vertex();
	rename.insert({other.s_state, p_start});

	/* Merge consolidated accept to 'other' start state */
	/* TODO should consolidation just point directly to 'other' start? */

	add_edge(*accept.begin(), p_start, NFAWeight(WeightType::EPSILON, 0));
	accept.clear();

	/* Merge result of other */

	std::queue<int> process;
	std::unordered_set<int> visited;

	process.push(other.s_state);

	while(!process.empty())
	{
		int current = process.front();
		process.pop();

		visited.insert(current);

		/* Fix accept states */
		if(other.is_accept(current))
			add_accept(rename[current]);

		const auto& edges = other.outgoing(current);

		for(auto& edge : edges)
		{
			if(rename.find(edge.get_dest()) == rename.end())
			{
				int vertex = make_vertex();
				rename.insert({edge.get_dest(), vertex});
			}

			add_edge(rename[current], rename[edge.get_dest()], edge.get_weight());

			if (visited.find(edge.get_dest()) == visited.end())
				process.push(edge.get_dest());
		}
	}
}

void NFA::consolidate()
{
	/* Nothing to do */
	if(accept.size() <= 1)
		return;

	int n_state = make_vertex();

	/* Point to new state */
	for (auto state : accept)
		add_edge(state, n_state, NFAWeight(WeightType::EPSILON, 0));

	/* Only one accept state */
	accept.clear();
	accept.insert(n_state);
}

bool NFA::add_accept(int state)
{
	if(!valid(state))
		return false;

	return accept.insert(state).second;
}

bool NFA::is_accept(int state)
{
	return accept.find(state) != accept.end();
}

bool NFA::set_start(int n_start)
{
	if(!valid(n_start))
		return false;

	s_state = n_start;
	return true;
}

int NFA::get_start()
{
	return s_state;
}

std::unordered_set<int> NFA::get_accept()
{
	return accept;
}

void NFA::clear_accept()
{
	accept.clear();
}

