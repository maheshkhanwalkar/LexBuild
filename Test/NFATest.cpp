
#include "FA/NFA.hpp"

#define BOOST_TEST_MODULE NFATest
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(nfa_transform_basic)
{
	NFA test(0);

	test.add_edge(0, 1, {0, Edge_Type::EPSILON});
	test.add_edge(0, 2, {0, Edge_Type::EPSILON});

	test.set_accept(2);
	std::unique_ptr<DFA> result = test.transform();

	BOOST_CHECK(result->at_accept());

	BOOST_CHECK(!result->has_edge(0, 1));
	BOOST_CHECK(!result->has_edge(0, 2));
}

BOOST_AUTO_TEST_CASE(nfa_transform_self_loop_simple)
{
	NFA test(0);

	test.add_edge(0, 0, {0, Edge_Type::EPSILON});
	test.add_edge(0, 0, {'a', Edge_Type::NORMAL});

	test.set_accept(0);
	std::unique_ptr<DFA> result = test.transform();

	BOOST_CHECK(result->at_accept());

	BOOST_CHECK(result->has_edge(0, 0));

	BOOST_CHECK(result->edges(0).size() == 1);
	BOOST_CHECK((*result->get_weight(0, 0)).front() == 'a');
}

BOOST_AUTO_TEST_CASE(nfa_transform_simple_alternation)
{
	NFA test(0);
	
	/* Simple (verbose) NFA for (a|b) */
	
	test.add_edge(0, 1, {0, Edge_Type::EPSILON});
	test.add_edge(0, 2, {0, Edge_Type::EPSILON});
	
	test.add_edge(1, 3, {'a', Edge_Type::NORMAL});
	test.add_edge(2, 4, {'b', Edge_Type::NORMAL});
	
	test.add_edge(3, 5, {0, Edge_Type::EPSILON});
	test.add_edge(4, 5, {0, Edge_Type::EPSILON});
	
	test.set_accept(5);
	
	std::unique_ptr<DFA> result = test.transform();

	BOOST_CHECK(!result->at_accept());
	BOOST_CHECK(result->edges(0).size() == 2);

	BOOST_CHECK(result->peek('a'));
	BOOST_CHECK(result->peek('b'));

	result->consume('a');

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->edges(result->c_state()).size() == 0);
	BOOST_CHECK(*(result->get_data()) == "a");

	result->reset();
	result->consume('b');

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->edges(result->c_state()).size() == 0);
	BOOST_CHECK(*(result->get_data()) == "b");
}

BOOST_AUTO_TEST_CASE(nfa_transform_simple_kleene)
{
	NFA test(0);

	/* Simple NFA for (a*) */

	test.add_edge(0, 1, {0, Edge_Type::EPSILON});
	test.add_edge(0, 3, {0, Edge_Type::EPSILON});

	test.add_edge(1, 2, {'a', Edge_Type::NORMAL});
	test.add_edge(2, 3, {0, Edge_Type::EPSILON});

	test.add_edge(3, 1, {0, Edge_Type::EPSILON});
	test.add_edge(3, 4, {0, Edge_Type::EPSILON});

	test.set_accept(4);

	std::unique_ptr<DFA> result = test.transform();

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->edges(0).size() == 1);

	BOOST_CHECK(result->peek('a'));
	result->consume('a');

	int current = result->c_state();

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->edges(current).size() == 1);

	BOOST_CHECK(result->peek('a'));
	result->consume('a');

	BOOST_CHECK(current == result->c_state());
}

BOOST_AUTO_TEST_CASE(nfa_transform_dup_edges)
{
	NFA test(0);

	test.add_edge(0, 1, {'v', Edge_Type::NORMAL});
	test.add_edge(0, 2, {'v', Edge_Type::NORMAL});

	test.add_edge(1, 3, {'a', Edge_Type::NORMAL});
	test.add_edge(2, 4, {'a', Edge_Type::NORMAL});

	test.add_edge(3, 5, {'r', Edge_Type::NORMAL});
	test.add_edge(4, 6, {'l', Edge_Type::NORMAL});

	test.set_accept(5);
	test.set_accept(6);

	std::unique_ptr<DFA> result = test.transform();

	BOOST_CHECK(!result->at_accept());
	BOOST_CHECK(result->edges(0).size() == 1);

	BOOST_CHECK(result->peek('v'));
	result->consume('v');

	BOOST_CHECK(!result->at_accept());
	BOOST_CHECK(result->edges(result->c_state()).size() == 1);

	BOOST_CHECK(result->peek('a'));
	result->consume('a');

	BOOST_CHECK(!result->at_accept());
	BOOST_CHECK(result->edges(result->c_state()).size() == 2);

	BOOST_CHECK(result->peek('r'));
	BOOST_CHECK(result->peek('l'));

	result->consume('r');

	int r_state = result->c_state();

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->edges(r_state).size() == 0);

	result->reset();

	result->consume('v');
	result->consume('a');
	result->consume('l');

	int l_state = result->c_state();

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->edges(l_state).size() == 0);

	BOOST_CHECK(l_state != r_state);
}