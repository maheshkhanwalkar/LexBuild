
#include "DFA.hpp"

DFA::DFA(int s_state)
{
	/* Initial DFA state */
	this->s_state = s_state;
	this->state = s_state;
}

bool DFA::peek(char c)
{
	return false;
}

bool DFA::at_accept()
{
	return accept.find(state) != accept.end();
}

void DFA::consume(char c)
{

}

std::shared_ptr<std::string> DFA::get_data()
{
	return std::make_shared<std::string>(current.str());
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

