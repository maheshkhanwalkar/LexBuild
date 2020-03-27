#include "nfa.hpp"

NFA NFA::make_simple(const std::string& word)
{
    NFA result;

    // This builds a very basic graph with two vertices and
    // a single connecting edge, as such:
    //
    //  -->(0)---word-->((1))
    //
    // The starting vertex is 0, which marks the entry point of
    // the NFA. The entirety of the word must match for the state
    // transition to occur from 0 to 1.
    //
    // The ending vertex 1 is marked as an accept state, which means
    // if, after processing all the text, the NFA is currently at this
    // state, then it accepts the input. Otherwise, it rejects it

    result.graph.add_edge(0, 1, word);

    // Indicate that the start state is vertex 0 -- this information is
    // crucial for NFA composition-based algorithms
    result.start = 0;

    // Indicate that vertex 1 is an accept state -- this information will
    // be used not only by simulation and transformation code, but also for
    // NFA composition-based algorithms
    result.accept.insert(1);

    // Add the allocated vertices to the used-vertex list and update the
    // vertex allocation counter -- these are used in more complicated NFA
    // construction schemes
    result.v_list.push_back(0);
    result.v_list.push_back(1);
    result.v_count = 2;

    return result;
}
