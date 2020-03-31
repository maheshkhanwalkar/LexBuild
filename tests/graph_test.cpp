#include "src/graph/digraph.hpp"
#include "gtest/gtest.h"

TEST(GraphBasics, Simple)
{
    digraph<int, char> graph;

    graph.add_edge(0, 1, 'a');

    ASSERT_TRUE(graph.has_edge(0, 1));
    ASSERT_FALSE(graph.has_edge(0, 2));
}

TEST(GraphBasics, Walk)
{
    digraph<int, char> graph;

    graph.add_edge(0, 1, 'a');
    graph.add_edge(1, 2, 'b');
    graph.add_edge(0, 2, 'b');

    std::unordered_set<int> res = graph.walk(0, 'a');

    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(*res.begin(), 1);

    res = graph.walk(0, 'b');

    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(*res.begin(), 2);

    res = graph.walk(0, 'c');
    ASSERT_TRUE(res.empty());

    res = graph.walk(1, 'b');

    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(*res.begin(), 2);
}

TEST(GraphBasics, Walk_DupEdge)
{
    digraph<int, char> graph;

    graph.add_edge(0, 1, 'a');
    graph.add_edge(0, 2, 'a');
    graph.add_edge(0, 3, 'a');
    graph.add_edge(0, 4, 'b');
    graph.add_edge(0, 5, 'b');

    std::unordered_set<int> a_walk = graph.walk(0, 'a');
    std::unordered_set<int> b_walk = graph.walk(0, 'b');

    ASSERT_EQ(a_walk.size(), 3);
    ASSERT_EQ(b_walk.size(), 2);
}

TEST(GraphBasics, Rename)
{
    digraph<int, char> graph;

    graph.add_edge(0, 1, 'a');
    graph.add_edge(0, 2, 'a');
    graph.add_edge(1, 2, 'b');
    graph.add_edge(1, 3, 'c');
    graph.add_edge(2, 4, 'a');
    graph.add_edge(4, 0, 'd');

    std::unordered_map<int, int> mapping;

    mapping[0] = 1;
    mapping[1] = 0;
    mapping[2] = 2;
    mapping[3] = 4;
    mapping[4] = 5;

    graph.rename_vertices(mapping);
    std::unordered_set<int> walk = graph.walk(1, 'a');

    ASSERT_EQ(walk.size(), 2);

    for(int elem : walk) {
        ASSERT_FALSE(elem != 0 && elem != 2);
    }

    ASSERT_TRUE(graph.has_edge(0, 4));
    ASSERT_TRUE(graph.has_edge(0, 2));
    ASSERT_TRUE(graph.has_edge(2, 5));
    ASSERT_TRUE(graph.has_edge(5, 1));
}
