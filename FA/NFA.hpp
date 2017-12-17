
#ifndef LEXBUILD_NFA_HPP
#define LEXBUILD_NFA_HPP

#include "Graph/WeightedDigraph.tpp"
#include "FA/DFA.hpp"

#include <memory>
#include <unordered_set>

enum class Edge_Type
{
	NORMAL, EPSILON
};

class NFA : public WeightedDigraph<std::pair<char, Edge_Type>>
{
public:

	/**
	 * Initialize a NFA
	 * @param s_state - start state
	 */
	explicit NFA(int s_state);

	/**
	 * Transform NFA to DFA
	 * @return the equivalent DFA
	 */
	std::unique_ptr<DFA> transform();

	/**
	 * Mark a state as an accept state
	 * @param state - state to mark
	 */
	void set_accept(int state);

	/**
	 * Check if a state is an accept state
	 * @param state - state to check
	 * @return true if state is an accept state
	 */
	bool is_accept(int state);

private:
	int s_state;
	std::unordered_set<int> accept;

	/* Compute unique edges of a NFA-DFA state */
	std::unique_ptr<std::unordered_map<char, std::unordered_set<int>>>
		unique_edges(const std::unordered_set<int>& node);

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
