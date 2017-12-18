
#define BOOST_TEST_MODULE RegexTest
#include <boost/test/included/unit_test.hpp>

#include "Regex/NFAGen.hpp"

BOOST_AUTO_TEST_CASE(nfagen_simple)
{
	std::string regex("this-is-a-test");
	std::unique_ptr<NFA> result = NFAGen::makeSimple(regex);

	int current = result->get_start();

	for(size_t s = 0; s < regex.size(); s++)
	{
		auto edges = result->edges(current);
		BOOST_CHECK(edges.size() == 1);

		auto edge = edges.front();

		BOOST_CHECK(edge.second.first == regex[s]);
		BOOST_CHECK(edge.second.second == Edge_Type::NORMAL);

		current = edge.first;
	}

	BOOST_CHECK(result->is_accept(current));
	BOOST_CHECK(result->edges(current).empty());
}

BOOST_AUTO_TEST_CASE(nfagen_simple_or)
{
	std::string r1("abcdefgh");
	std::string r2("ijklmnop");

	auto g1 = NFAGen::makeSimple(r1);
	auto g2 = NFAGen::makeSimple(r2);

	NFAGen::makeOr(*g1, *g2);

	/* Ensure no changes to 'g2' */

	int current = g2->get_start();

	for(size_t s = 0; s < r2.size(); s++)
	{
		auto edges = g2->edges(current);
		BOOST_CHECK(edges.size() == 1);

		auto edge = edges.front();

		BOOST_CHECK(edge.second.first == r2[s]);
		BOOST_CHECK(edge.second.second == Edge_Type::NORMAL);

		current = edge.first;
	}

	BOOST_CHECK(g2->is_accept(current));
	BOOST_CHECK(g2->edges(current).empty());

	/* Check changes to 'g1' */

	int start = g1->get_start();
	BOOST_CHECK(g1->edges(start).size() == 2);

	for(auto item : g1->edges(start))
		BOOST_CHECK(item.second.second == Edge_Type::EPSILON);

	int left = g1->edges(start).front().first;
	int right = g1->edges(start).back().first;

	for(size_t s = 0; s < r1.size(); s++)
	{
		auto edges = g1->edges(left);
		BOOST_CHECK(edges.size() == 1);

		auto edge = edges.front();

		BOOST_CHECK(edge.second.first == r1[s]);
		BOOST_CHECK(edge.second.second == Edge_Type::NORMAL);

		left = edge.first;
	}

	BOOST_CHECK(!g1->is_accept(left));
	BOOST_CHECK(g1->edges(left).size() == 1);

	int a_left = g1->edges(left).front().first;

	BOOST_CHECK(g1->is_accept(a_left));
	BOOST_CHECK(g1->edges(a_left).empty());

	for(size_t s = 0; s < r2.size(); s++)
	{
		auto edges = g1->edges(right);
		BOOST_CHECK(edges.size() == 1);

		auto edge = edges.front();

		BOOST_CHECK(edge.second.first == r2[s]);
		BOOST_CHECK(edge.second.second == Edge_Type::NORMAL);

		right = edge.first;
	}

	BOOST_CHECK(!g1->is_accept(right));
	BOOST_CHECK(g1->edges(right).size() == 1);

	int a_right = g1->edges(right).front().first;

	BOOST_CHECK(g1->is_accept(a_right));
	BOOST_CHECK(g1->edges(a_right).empty());

	BOOST_CHECK(a_left == a_right);
}
