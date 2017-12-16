
#include "FA/NFA.hpp"

#define BOOST_TEST_MODULE NFATest
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(nfa_transform_basic)
{
	NFA test(0);

	test.add_edge(0, 1, {0, Edge_Type::EPSILON});
	test.add_edge(0, 2, {0, Edge_Type::EPSILON});

	test.set_accept(2);
	std::shared_ptr<DFA> result = test.transform();

	BOOST_CHECK(result->at_accept());

	BOOST_CHECK(!result->has_edge(0, 1));
	BOOST_CHECK(!result->has_edge(0, 2));
}