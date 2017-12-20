
#ifndef LEXBUILD_NFA_HPP
#define LEXBUILD_NFA_HPP

#include "Graph/Digraph.hpp"
#include "FA/DFA.hpp"

#include "FA/Edge/NFAWeight.hpp"

#include <memory>
#include <unordered_set>

class NFA : public Digraph<NFAWeight>
{
public:

	/**
	 * Initialize the NFA
	 */
	explicit NFA();

	/**
	 * Transform NFA to DFA
	 * @return the equivalent DFA
	 */
	std::unique_ptr<DFA> transform();

	/**
	 * Change the NFA start state
	 * @param n_start - new start state
	 * @return true, if successful
	 */
	bool set_start(int n_start);

	/**
	 * Start state of the NFA
	 * @return the start state
	 */
	int get_start();

	/**
	 * Mark a state as an accept state
	 * @param state - state to mark
	 * @return true, if the state was successfully marked
	 */
	bool add_accept(int state);

	/**
	 * Check if a state is an accept state
	 * @param state - state to check
	 * @return true if state is an accept state
	 */
	bool is_accept(int state);

	/**
	 * Returns the accept set of the NFA
	 * @return NFA accept set
	 */
	std::unordered_set<int> get_accept();

	/**
	 * Clears the accept set of the NFA
	 */
	void clear_accept();

	/**
	 * Merge an NFA into this graph at a specific location
	 *
	 * Approach:
	 *   1. Merge 'other' into this graph and apply renaming as
	 *      required
	 *
	 *   2. Make an e-transition between 'where' to start of 'other'
	 *
	 *   3. Add new accept states, but *do not* remove any. This is
	 *      different than the merge() behavior.
	 *
	 * @param other - NFA to merge with
	 * @param where - vertex to attach 'other' to.
	 * @return true, if the merge was successful
	 */
	bool merge_at(NFA& other, int where);

	/**
	 * Merge an NFA into this graph:
	 *
	 * Approach:
	 *   1. Consolidate accept states into one via e-transitions
	 *
	 *   2. Merge 'other' into this graph and apply renaming as
	 *      required.
	 *
	 *   3. Make an edge between consolidated accept state and
	 *      copied start state of 'other'
	 *
	 * @param other - NFA to merge with
	 */
	void merge(NFA& other);

	/**
	 * Consolidate accept states
	 *
	 *   All previous accept states point to *one* true accept state
	 *   via e-transitions.
	 */
	void consolidate();

private:
	/* Start state */
	int s_state;

	/* Accept states */
	std::unordered_set<int> accept;

	/* Compute unique edges of a "DFA" state */
	std::unique_ptr<std::unordered_map<char, std::unordered_set<int>>>
		unique_edges(const std::unordered_set<int>& node);

	/* Compute equivalent (expanded) DFA state */
	std::unordered_set<int> expand(int state);

	/* Hash function for unordered_set */
	struct set_hash
	{
		size_t operator() (const std::unordered_set<int>& set) const
		{
			std::hash<int> h_func;
			size_t sum = 0;

			for (auto item : set)
				sum += h_func(item);

			return sum;
		}
	};
};


#endif
