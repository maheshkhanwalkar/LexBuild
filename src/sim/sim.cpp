#include "src/graph/digraph.hpp"
#include <iostream>

int main()
{
    digraph<int, char> graph;

    graph.add_edge(0, 1, 'a');
    graph.add_edge(0, 1, 'b');

    std::cout << graph.has_edge(0, 1) << std::endl;
    std::cout << graph.has_edge(0, 2) << std::endl;
}
