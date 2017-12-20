
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
		auto edges = result->outgoing(current);
		BOOST_CHECK(edges.size() == 1);

		auto edge = (*edges.begin());

		BOOST_CHECK(edge.get_weight().get_weight() == regex[s]);
		BOOST_CHECK(edge.get_weight().get_type() == WeightType::NORMAL);

		current = edge.get_dest();
	}

	BOOST_CHECK(result->is_accept(current));
	BOOST_CHECK(result->outdegree(current) == 0);
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
		auto edges = g2->outgoing(current);
		BOOST_CHECK(edges.size() == 1);

		auto edge = (*edges.begin());

		BOOST_CHECK(edge.get_weight().get_weight() == r2[s]);
		BOOST_CHECK(edge.get_weight().get_type() == WeightType::NORMAL);

		current = edge.get_dest();
	}

	BOOST_CHECK(g2->is_accept(current));
	BOOST_CHECK(g2->outdegree(current) == 0);

	/* Check changes to 'g1' */

	int start = g1->get_start();
	BOOST_CHECK(g1->outdegree(start) == 2);

	for(const auto& item : g1->outgoing(start))
		BOOST_CHECK(item.get_weight().get_type() == WeightType::EPSILON);

	int left = -1, right = -1;

	for(const auto& item : g1->outgoing(start))
	{
		if(left == -1)
			left = item.get_dest();
		else
			right = item.get_dest();
	}

	for(size_t s = 0; s < r1.size(); s++)
	{
		auto edges = g1->outgoing(left);
		BOOST_CHECK(edges.size() == 1);

		auto edge = *edges.begin();

		BOOST_CHECK(edge.get_weight().get_weight() == r1[s]);
		BOOST_CHECK(edge.get_weight().get_type() == WeightType::NORMAL);

		left = edge.get_dest();
	}

	BOOST_CHECK(!g1->is_accept(left));
	BOOST_CHECK(g1->outdegree(left) == 1);

	int a_left = (*g1->outgoing(left).begin()).get_dest();

	BOOST_CHECK(g1->is_accept(a_left));
	BOOST_CHECK(g1->outdegree(a_left) == 0);

	for(size_t s = 0; s < r2.size(); s++)
	{
		auto edges = g1->outgoing(right);
		BOOST_CHECK(edges.size() == 1);

		auto edge = *edges.begin();

		BOOST_CHECK(edge.get_weight().get_weight() == r2[s]);
		BOOST_CHECK(edge.get_weight().get_type() == WeightType::NORMAL);

		right = edge.get_dest();
	}

	BOOST_CHECK(!g1->is_accept(right));
	BOOST_CHECK(g1->outdegree(right) == 1);

	int a_right = (*g1->outgoing(right).begin()).get_dest();

	BOOST_CHECK(g1->is_accept(a_right));
	BOOST_CHECK(g1->outdegree(a_right) == 0);

	BOOST_CHECK(a_left == a_right);
}
