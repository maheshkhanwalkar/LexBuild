
#include "DFA.hpp"

DFA::DFA()
{
	/* Initial DFA state */
	this->s_state = make_vertex();
	this->state = s_state;

	this->invalid = false;
}

bool DFA::set_start(int n_start)
{
	if(!valid(n_start))
		return false;

	if(state != s_state)
		return false;

	s_state = n_start;
	state = n_start;

	return true;
}

bool DFA::peek(char c)
{
	if(invalid)
		return false;

	for(auto edge : outgoing(state))
	{
		if(edge.get_weight() == c)
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

	for(auto edge : outgoing(state))
	{
		if(edge.get_weight() == c)
		{
			state = edge.get_dest();
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

bool DFA::add_accept(int state)
{
	if(!valid(state))
		return false;

	return accept.insert(state).second;
}

void DFA::reset()
{
	state = s_state;
	current.clear();
}

int DFA::curr_state()
{
	return state;
}

