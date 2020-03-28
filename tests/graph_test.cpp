#include "src/graph/digraph.hpp"
#include "gtest/gtest.h"

TEST(GraphBasics, Simple)
{
    digraph<int, char> graph;

    graph.add_edge(0, 1, 'a');

    ASSERT_TRUE(graph.has_edge(0, 1));
    ASSERT_FALSE(graph.has_edge(0, 2));
}
