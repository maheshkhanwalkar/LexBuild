#include "src/graph/digraph.hpp"
#include "src/automata/nfa.hpp"

#include <iostream>

int main()
{
    digraph<int, char> graph;

    graph.add_edge(0, 1, 'a');
    graph.add_edge(0, 2, 'a');
    graph.add_edge(0, 3, 'b');

    std::unordered_set<int> reach = graph.walk(0, 'a');

    // std::cout << graph.has_edge(0, 1) << std::endl;
    // std::cout << graph.has_edge(0, 2) << std::endl;

    for(int elem : reach) {
        std::cout << elem << std::endl;
    }

    return 0;
}
