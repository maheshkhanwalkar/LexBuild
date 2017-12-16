
#include "Graph/WeightedDigraph.tpp"

#define BOOST_TEST_MODULE DigraphTest
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(digraph_test_add)
{
	WeightedDigraph<int> test;

	for (int i = 0; i < 1000; i++)
		test.add_edge(i, i + 1, i * 2);

	for (int i = 0; i < 1000; i++)
		BOOST_CHECK(!test.has_edge(i + 1, i));
}

BOOST_AUTO_TEST_CASE(digraph_test_remove)
{
	WeightedDigraph<int> test;

	for (int i = 0; i < 1000; i++)
		test.add_edge(i, i + 1, i * 2);

	for (int i = 100; i < 350; i += 2)
		test.remove_edge(i, i + 1);

	for (int i = 100; i < 350; i += 2)
		BOOST_CHECK(!test.has_edge(i, i + 1));
}

BOOST_AUTO_TEST_CASE(digraph_test_weight)
{
	WeightedDigraph<int> test;

	for (int i = 0; i < 1000; i++)
		test.add_edge(i, i + 1, i * 2);

	for (int i = 1000; i > 0; i--)
	{
		BOOST_CHECK(*test.get_weight(i - 1, i) == (i - 1) * 2);
		BOOST_CHECK(test.get_weight(i, i - 1) == nullptr);
	}
}