
#ifndef LEXBUILD_DFA_HPP
#define LEXBUILD_DFA_HPP

#include "Graph/Digraph.hpp"

#include <sstream>
#include <unordered_set>

class DFA : public Digraph<char>
{
public:
	/**
	 * Initialize the DFA
	 */
	explicit DFA();

	/**
	 * Change the start state of the DFA
	 * @param n_start - new start state
	 * @return true, if successful
	 */
	bool set_start(int n_start);

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
	 * Current DFA state
	 * @return current state
	 */
	int curr_state();

	/**
	 * Get the processed data
	 * @return the data
	 */
	std::unique_ptr<std::string> get_data();

	/**
	 * Mark a state as an accept state
	 * @param state - state to mark
	 * @return true, if the state was successfully marked
	 */
	bool add_accept(int state);

	/**
	 * Reset the DFA
	 */
	void reset();

private:
	std::string current;

	int s_state;
	int state;

	std::unordered_set<int> accept;
	bool invalid;
};


#endif
