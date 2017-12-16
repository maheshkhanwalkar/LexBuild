
#ifndef LEXBUILD_DFA_HPP
#define LEXBUILD_DFA_HPP

#include "Graph/WeightedDigraph.tpp"
#include <sstream>
#include <unordered_set>

class DFA : public WeightedDigraph<char>
{
public:
	/**
	 * Initialize a DFA
	 * @param s_state - start state of the DFA
	 */
	explicit DFA(int s_state);

	/**
	 * Peek to see if DFA can transition on 'c'
	 * @return true if transition is possible
	 */
	bool peek(char c);

	/**
	 * Check if DFA is in the accept state
	 * @return true if DFA is in an accept state
	 */
	bool at_accept();

	/**
	 * Consume 'c' and perform state transition
	 * @param c - character to process
	 */
	void consume(char c);

	/**
	 * Get the processed data
	 * @return the data
	 */
	std::shared_ptr<std::string> get_data();

	/**
	 * Mark a state as an accept state
	 * @param state - state to mark
	 */
	void set_accept(int state);

	/**
	 * Reset the DFA
	 */
	void reset();

private:
	std::stringstream current;

	int s_state;
	int state;

	std::unordered_set<int> accept;
};


#endif
