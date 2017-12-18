
#include "FA/NFA.hpp"

#define BOOST_TEST_MODULE NFATest
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(nfa_transform_basic)
{
	NFA test(0);

	test.add_edge(0, 1, {0, Edge_Type::EPSILON});
	test.add_edge(0, 2, {0, Edge_Type::EPSILON});

	test.add_accept(2);
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

	test.add_accept(0);
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

	test.add_accept(5);
	
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

BOOST_AUTO_TEST_CASE(nfa_transform_simple_star)
{
	NFA test(0);

	/* Simple NFA for (a*) */

	test.add_edge(0, 1, {0, Edge_Type::EPSILON});
	test.add_edge(0, 3, {0, Edge_Type::EPSILON});

	test.add_edge(1, 2, {'a', Edge_Type::NORMAL});
	test.add_edge(2, 3, {0, Edge_Type::EPSILON});

	test.add_edge(3, 1, {0, Edge_Type::EPSILON});
	test.add_edge(3, 4, {0, Edge_Type::EPSILON});

	test.add_accept(4);

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

	test.add_accept(5);
	test.add_accept(6);

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

BOOST_AUTO_TEST_CASE(nfa_consolidate)
{
	NFA test(0);

	test.add_edge(0, 1, {'a', Edge_Type::NORMAL});
	test.add_edge(0, 2, {'b', Edge_Type::NORMAL});
	test.add_edge(0, 3, {'c', Edge_Type::NORMAL});

	for(int i = 1; i <= 3; i++)
		test.add_accept(i);

	test.consolidate();

	BOOST_CHECK(!test.is_accept(0));
	BOOST_CHECK(test.edges(0).size() == 3);

	for(auto edge : test.edges(0))
		BOOST_CHECK(edge.second.second == Edge_Type::NORMAL);

	for(int i = 1; i <= 3; i++)
	{
		BOOST_CHECK(!test.is_accept(i));
		BOOST_CHECK(test.edges(i).size() == 1);

		auto pair = test.edges(i).front();

		BOOST_CHECK(pair.first == 4);
		BOOST_CHECK(pair.second.second == Edge_Type::EPSILON);
	}

	BOOST_CHECK(test.is_accept(4));
	BOOST_CHECK(test.edges(4).size() == 0);
}

BOOST_AUTO_TEST_CASE(nfa_merge_no_consolidate_simple)
{
	NFA g1(0);

	g1.add_edge(0, 1, {'a', Edge_Type::NORMAL});
	g1.add_edge(1, 2, {'b', Edge_Type::NORMAL});

	g1.add_accept(2);

	NFA g2(0);

	g2.add_edge(0, 0, {'c', Edge_Type::NORMAL});
	g2.add_accept(0);

	g1.merge(g2);

	/* Make sure g2 is *not* modified */
	BOOST_CHECK(g2.is_accept(0));
	BOOST_CHECK(g2.edges(0).size() == 1);

	auto pair = g2.edges(0).front();

	BOOST_CHECK(pair.first == 0);
	BOOST_CHECK(pair.second.first == 'c');
	BOOST_CHECK(pair.second.second == Edge_Type::NORMAL);

	/* Check modifications to g1 */
	BOOST_CHECK(!g1.is_accept(2));
	BOOST_CHECK(g1.edges(2).size() == 1);

	pair = g1.edges(2).front();

	BOOST_CHECK(pair.first == 3);
	BOOST_CHECK(pair.second.second == Edge_Type::EPSILON);

	BOOST_CHECK(g1.is_accept(3));
	BOOST_CHECK(g1.edges(3).size() == 1);

	pair = g1.edges(3).front();

	BOOST_CHECK(pair.first == 3);
	BOOST_CHECK(pair.second.first == 'c');
	BOOST_CHECK(pair.second.second == Edge_Type::NORMAL);
}

BOOST_AUTO_TEST_CASE(nfa_merge_consolidate)
{
	NFA g1(0);

	g1.add_edge(0, 1, {'a', Edge_Type::NORMAL});
	g1.add_edge(0, 2, {'b', Edge_Type::NORMAL});

	g1.add_accept(1);
	g1.add_accept(2);

	NFA g2(0);

	g2.add_edge(0, 1, {'c', Edge_Type::NORMAL});
	g2.add_edge(0, 2, {'d', Edge_Type::NORMAL});

	g2.add_accept(1);
	g2.add_accept(2);

	g1.merge(g2);

	/* Make sure g2 is *not* modified */
	BOOST_CHECK(g2.is_accept(1));
	BOOST_CHECK(g2.is_accept(2));

	BOOST_CHECK(g2.edges(0).size() == 2);

	for(auto edge : g2.edges(0))
		BOOST_CHECK(edge.second.second == Edge_Type::NORMAL);

	/* Check modifications */
	BOOST_CHECK(!g1.is_accept(1));
	BOOST_CHECK(!g1.is_accept(2));

	BOOST_CHECK(g1.edges(1).size() == 1);
	BOOST_CHECK(g1.edges(2).size() == 1);

	BOOST_CHECK(g1.edges(1).front().first == 3);
	BOOST_CHECK(g1.edges(1).front().second.second == Edge_Type::EPSILON);

	BOOST_CHECK(g1.edges(2).front().first == 3);
	BOOST_CHECK(g1.edges(2).front().second.second == Edge_Type::EPSILON);

	BOOST_CHECK(g1.edges(3).size() == 1);
	BOOST_CHECK(g1.edges(3).front().first == 4);
	BOOST_CHECK(g1.edges(3).front().second.second == Edge_Type::EPSILON);

	BOOST_CHECK(g1.edges(4).size() == 2);

	for(auto edge : g1.edges(4))
		BOOST_CHECK(edge.second.second == Edge_Type::NORMAL);

	BOOST_CHECK(g1.is_accept(5));
	BOOST_CHECK(g1.is_accept(6));

	BOOST_CHECK(g1.edges(5).size() == 0);
	BOOST_CHECK(g1.edges(6).size() == 0);
}

BOOST_AUTO_TEST_CASE(nfa_merge_at)
{
	NFA g1(0);

	g1.add_edge(0, 1, {'a', Edge_Type::NORMAL});
	g1.add_accept(1);

	NFA g2(0);

	g2.add_edge(0, 1, {'b', Edge_Type::NORMAL});
	g2.add_accept(1);

	g1.merge_at(g2, 0);

	/* No modifications of 'g2' */

	BOOST_CHECK(g2.is_accept(1));
	BOOST_CHECK(g2.edges(0).size() == 1);
	BOOST_CHECK(g2.edges(0).front().first == 1);
	BOOST_CHECK(g2.edges(0).front().second.second == Edge_Type::NORMAL);

	/* Check 'g1' modifications */

	BOOST_CHECK(g1.get_accept().size() == 2);
	BOOST_CHECK(g1.edges(0).size() == 2);

	BOOST_CHECK(g1.is_accept(1));
	BOOST_CHECK(g1.is_accept(3));
}
