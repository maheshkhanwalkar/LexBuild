
#include "Graph/Digraph.hpp"

#define BOOST_TEST_MODULE DigraphTest
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(digraph_test_add)
{
	Digraph<int> test;
	std::vector<int> vertices;

	for(int i = 0; i <= 1000; i++)
		vertices.push_back(test.make_vertex());

	for(int i = 0; i < 1000; i++)
		BOOST_CHECK(test.add_edge(vertices[i], vertices[i + 1], i * 2));

	for (int i = 0; i < 1000; i++)
	{
		BOOST_CHECK(test.has_edge(vertices[i], vertices[i + 1]));
		BOOST_CHECK(!test.has_edge(vertices[i + 1], vertices[i]));

		BOOST_CHECK(test.outdegree(vertices[i]) == 1);
	}
}

BOOST_AUTO_TEST_CASE(digraph_test_remove)
{
	Digraph<int> test;
	std::vector<int> vertices;

	for(int i = 0; i <= 1000; i++)
		vertices.push_back(test.make_vertex());

	for (int i = 0; i < 1000; i++)
		BOOST_CHECK(test.add_edge(vertices[i], vertices[i + 1], i * 2));

	for (int i = 100; i < 350; i += 2)
		BOOST_CHECK(test.remove_edge(vertices[i], vertices[i + 1]));

	for (int i = 100; i < 350; i += 2)
	{
		BOOST_CHECK(!test.has_edge(vertices[i], vertices[i + 1]));
		BOOST_CHECK(!test.has_edge(vertices[i + 1], vertices[i]));
	}
}

BOOST_AUTO_TEST_CASE(digraph_test_has_edge)
{
	Digraph<int> test;
	std::vector<int> vertices;

	for(int i = 0; i <= 1000; i++)
		vertices.push_back(test.make_vertex());

	for (int i = 0; i < 1000; i++)
	{
		if(i % 2 == 0)
			BOOST_CHECK(test.add_edge(vertices[i], vertices[i + 1], i * 2));
	}

	for (int i = 0; i < 1000; i++)
	{
		if (i % 2 == 0)
		{
			BOOST_CHECK(test.has_edge(vertices[i], vertices[i + 1]));
			BOOST_CHECK(test.outdegree(vertices[i]) == 1);
		}
		else
		{
			BOOST_CHECK(!test.has_edge(vertices[i], vertices[i + 1]));
			BOOST_CHECK(test.outdegree(vertices[i]) == 0);
		}
	}
}

BOOST_AUTO_TEST_CASE(digraph_test_connections)
{
	Digraph<int> test;
	std::vector<int> vertices;

	for (int i = 0; i <= 1000; i++)
		vertices.push_back(test.make_vertex());

	for (int i = 0; i < 1000; i++)
		BOOST_CHECK(test.add_edge(vertices[i], vertices[i + 1], i * 2));

	for(int i = 0; i < 1000; i++)
	{
		BOOST_CHECK(test.outdegree(vertices[i]) == 1);
		BOOST_CHECK(test.indegree(vertices[i + 1]) == 1);

		BOOST_CHECK(test.has_edge(vertices[i], vertices[i + 1]));
		BOOST_CHECK(!test.has_edge(vertices[i + 1], vertices[i]));
	}
}
