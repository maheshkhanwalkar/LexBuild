
#include "DFA.hpp"

DFA::DFA(int s_state)
{
	/* Initial DFA state */
	this->s_state = s_state;
	this->state = s_state;

	this->invalid = false;
}

bool DFA::peek(char c)
{
	if(invalid)
		return false;

	for(auto edge : adj_list[state])
	{
		if(edge.second == c)
			return true;
	}

	return false;
}

bool DFA::at_accept()
{
	return accept.find(state) != accept.end();
}

void DFA::consume(char c)
{
	if(invalid)
		return;

	for(auto edge : adj_list[state])
	{
		if(edge.second == c)
		{
			state = edge.first;
			current += c;

			return;
		}
	}

	invalid = true;
}

std::unique_ptr<std::string> DFA::get_data()
{
	return std::make_unique<std::string>(current);
}

void DFA::set_accept(int state)
{
	accept.insert(state);
}

void DFA::reset()
{
	state = s_state;
	current.clear();
}

int DFA::c_state()
{
	return state;
}

