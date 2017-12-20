
#include "FA/NFA.hpp"

#define BOOST_TEST_MODULE NFATest
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(nfa_transform_basic)
{
	NFA test;

	int start = test.make_vertex();

	int first = test.make_vertex();
	int second = test.make_vertex();

	test.set_start(start);

	test.add_edge(start, first, NFAWeight());
	test.add_edge(start, second, NFAWeight());

	test.add_accept(second);
	std::unique_ptr<DFA> result = test.transform();

	BOOST_CHECK(result->at_accept());

	BOOST_CHECK(!result->has_edge(start, first));
	BOOST_CHECK(!result->has_edge(start, second));
}

BOOST_AUTO_TEST_CASE(nfa_transform_self_loop_simple)
{
	NFA test;
	int start = test.make_vertex();

	test.set_start(start);

	test.add_edge(start, start, NFAWeight());
	test.add_edge(start, start, NFAWeight('a'));

	test.add_accept(start);
	std::unique_ptr<DFA> result = test.transform();

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->has_edge(start, start));

	BOOST_CHECK(result->outdegree(start) == 1);
	BOOST_CHECK((*(result->outgoing(start).begin())).get_weight() == 'a');
}

BOOST_AUTO_TEST_CASE(nfa_transform_simple_alternation)
{
	NFA test;

	int start = test.make_vertex();
	test.set_start(start);

	/* Simple (verbose) NFA for (a|b) */

	int v[6];
	v[0] = start;

	for(int i = 1; i <= 5; i++)
		v[i] = test.make_vertex();

	test.add_edge(start, v[1], NFAWeight());
	test.add_edge(start, v[2], NFAWeight());

	test.add_edge(v[1], v[3], NFAWeight('a'));
	test.add_edge(v[2], v[4], NFAWeight('b'));

	test.add_edge(v[3], v[5], NFAWeight());
	test.add_edge(v[4], v[5], NFAWeight());

	test.add_accept(v[5]);
	
	std::unique_ptr<DFA> result = test.transform();

	BOOST_CHECK(!result->at_accept());
	BOOST_CHECK(result->outdegree(start) == 2);

	BOOST_CHECK(result->peek('a'));
	BOOST_CHECK(result->peek('b'));

	result->consume('a');

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->outdegree(result->curr_state()) == 0);

	BOOST_CHECK(*(result->get_data()) == "a");

	result->reset();
	result->consume('b');

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->outdegree(result->curr_state()) == 0);

	BOOST_CHECK(*(result->get_data()) == "b");
}

BOOST_AUTO_TEST_CASE(nfa_transform_simple_star)
{
	NFA test;

	int start = test.make_vertex();
	test.set_start(start);

	int v[5];

	v[0] = start;

	for(int i = 1; i <= 4; i++)
		v[i] = test.make_vertex();

	/* Simple NFA for (a*) */

	test.add_edge(v[0], v[1], NFAWeight());
	test.add_edge(v[0], v[3], NFAWeight());

	test.add_edge(v[1], v[2], NFAWeight('a'));
	test.add_edge(v[2], v[3], NFAWeight());

	test.add_edge(v[3], v[1], NFAWeight());
	test.add_edge(v[3], v[4], NFAWeight());

	test.add_accept(v[4]);

	std::unique_ptr<DFA> result = test.transform();

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->outdegree(start) == 1);

	BOOST_CHECK(result->peek('a'));
	result->consume('a');

	int current = result->curr_state();

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->outdegree(current) == 1);

	BOOST_CHECK(result->peek('a'));
	result->consume('a');

	BOOST_CHECK(current == result->curr_state());
}

BOOST_AUTO_TEST_CASE(nfa_transform_dup_edges)
{
	NFA test;

	int start = test.make_vertex();
	test.set_start(start);

	int v[7];
	v[0] = start;

	for(int i = 1; i <= 6; i++)
		v[i] = test.make_vertex();

	test.add_edge(v[0], v[1], NFAWeight('v'));
	test.add_edge(v[0], v[2], NFAWeight('v'));

	test.add_edge(v[1], v[3], NFAWeight('a'));
	test.add_edge(v[2], v[4], NFAWeight('a'));

	test.add_edge(v[3], v[5], NFAWeight('r'));
	test.add_edge(v[4], v[6], NFAWeight('l'));

	test.add_accept(v[5]);
	test.add_accept(v[6]);

	std::unique_ptr<DFA> result = test.transform();

	BOOST_CHECK(!result->at_accept());
	BOOST_CHECK(result->outdegree(v[0]) == 1);

	BOOST_CHECK(result->peek('v'));
	result->consume('v');

	BOOST_CHECK(!result->at_accept());
	BOOST_CHECK(result->outdegree(result->curr_state()) == 1);

	BOOST_CHECK(result->peek('a'));
	result->consume('a');

	BOOST_CHECK(!result->at_accept());
	BOOST_CHECK(result->outdegree(result->curr_state()) == 2);

	BOOST_CHECK(result->peek('r'));
	BOOST_CHECK(result->peek('l'));

	result->consume('r');

	int r_state = result->curr_state();

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->outdegree(r_state) == 0);

	result->reset();

	result->consume('v');
	result->consume('a');
	result->consume('l');

	int l_state = result->curr_state();

	BOOST_CHECK(result->at_accept());
	BOOST_CHECK(result->outdegree(l_state) == 0);

	BOOST_CHECK(l_state != r_state);
}

BOOST_AUTO_TEST_CASE(nfa_consolidate)
{
	NFA test;

	int start = test.make_vertex();
	test.set_start(start);

	int v[4];
	v[0] = start;

	for(int i = 1; i <= 3; i++)
		v[i] = test.make_vertex();

	test.add_edge(v[0], v[1], NFAWeight('a'));
	test.add_edge(v[0], v[2], NFAWeight('b'));
	test.add_edge(v[0], v[3], NFAWeight('c'));

	for(int i = 1; i <= 3; i++)
		test.add_accept(v[i]);

	test.consolidate();

	BOOST_CHECK(!test.is_accept(v[0]));
	BOOST_CHECK(test.outdegree(v[0]) == 3);

	for(auto edge : test.outgoing(v[0]))
		BOOST_CHECK(edge.get_weight().get_type() == WeightType::NORMAL);

	int dest = -1;

	for(int i = 1; i <= 3; i++)
	{
		BOOST_CHECK(!test.is_accept(v[i]));
		BOOST_CHECK(test.outdegree(v[i]) == 1);

		auto pair = (*test.outgoing(v[i]).begin());

		if(dest != -1)
			BOOST_CHECK(dest == pair.get_dest());
		else
			dest = pair.get_dest();

		BOOST_CHECK(pair.get_weight().get_type() == WeightType::EPSILON);
	}

	BOOST_CHECK(test.is_accept(dest));
	BOOST_CHECK(test.outdegree(dest) == 0);
}


BOOST_AUTO_TEST_CASE(nfa_merge_no_consolidate_simple)
{
	NFA g1;
	int v[3];

	for(int i = 0; i < 3; i++)
		v[i] = g1.make_vertex();

	g1.set_start(v[0]);

	g1.add_edge(v[0], v[1], NFAWeight('a'));
	g1.add_edge(v[1], v[2], NFAWeight('b'));

	g1.add_accept(v[2]);

	NFA g2;
	int start = g2.make_vertex();

	g2.set_start(start);

	g2.add_edge(start, start, NFAWeight('c'));
	g2.add_accept(start);

	g1.merge(g2);

	/* Make sure g2 is *not* modified */
	BOOST_CHECK(g2.is_accept(start));
	BOOST_CHECK(g2.outdegree(start) == 1);

	auto pair = (*g2.outgoing(start).begin());

	BOOST_CHECK(pair.get_dest() == start);
	BOOST_CHECK(pair.get_weight().get_weight() == 'c');
	BOOST_CHECK(pair.get_weight().get_type() == WeightType::NORMAL);

	/* Check modifications to g1 */
	BOOST_CHECK(!g1.is_accept(v[2]));
	BOOST_CHECK(g1.outdegree(v[2]) == 1);

	pair = (*g1.outgoing(v[2]).begin());

	int dest = pair.get_dest();
	BOOST_CHECK(pair.get_weight().get_type() == WeightType::EPSILON);

	BOOST_CHECK(g1.is_accept(dest));
	BOOST_CHECK(g1.outdegree(dest) == 1);

	pair = (*g1.outgoing(dest).begin());

	BOOST_CHECK(pair.get_weight().get_weight() == 'c');
	BOOST_CHECK(pair.get_weight().get_type() == WeightType::NORMAL);
}

BOOST_AUTO_TEST_CASE(nfa_merge_consolidate)
{
	NFA g1;

	int g1s = g1.make_vertex();
	g1.set_start(g1s);

	int first = g1.make_vertex();
	int second = g1.make_vertex();

	g1.add_edge(g1s, first, NFAWeight('a'));
	g1.add_edge(g1s, second, NFAWeight('b'));

	g1.add_accept(first);
	g1.add_accept(second);

	NFA g2;

	int g2s = g2.make_vertex();
	g2.set_start(g2s);

	int g2first = g2.make_vertex();
	int g2second = g2.make_vertex();

	g2.add_edge(g2s, g2first, NFAWeight('c'));
	g2.add_edge(g2s, g2second, NFAWeight('d'));

	g2.add_accept(g2first);
	g2.add_accept(g2second);

	g1.merge(g2);

	/* Make sure g2 is *not* modified */
	BOOST_CHECK(g2.is_accept(g2first));
	BOOST_CHECK(g2.is_accept(g2second));

	BOOST_CHECK(g2.outdegree(g2s) == 2);

	for(auto edge : g2.outgoing(g2s))
		BOOST_CHECK(edge.get_weight().get_type() == WeightType::NORMAL);

	/* Check modifications */
	BOOST_CHECK(!g1.is_accept(first));
	BOOST_CHECK(!g1.is_accept(second));

	BOOST_CHECK(g1.outdegree(first) == 1);
	BOOST_CHECK(g1.outdegree(second) == 1);

	BOOST_CHECK(g1.get_accept().size() == 2);

	for(int vertex : g1.get_accept())
		BOOST_CHECK(g1.outdegree(vertex) == 0);
}

BOOST_AUTO_TEST_CASE(nfa_merge_at)
{
	NFA g1;

	int g1s = g1.make_vertex();
	g1.set_start(g1s);

	int g1_one = g1.make_vertex();

	g1.add_edge(g1s, g1_one, NFAWeight('a'));
	g1.add_accept(g1_one);

	NFA g2;

	int g2s = g2.make_vertex();
	g2.set_start(g2s);

	int g2one = g2.make_vertex();

	g2.add_edge(g2s, g2one, NFAWeight('b'));
	g2.add_accept(g2one);

	g1.merge_at(g2, g1s);

	/* No modifications of 'g2' */

	BOOST_CHECK(g2.is_accept(g2one));
	BOOST_CHECK(g2.outdegree(g2s) == 1);

	/* Check 'g1' modifications */

	BOOST_CHECK(g1.get_accept().size() == 2);
	BOOST_CHECK(g1.outdegree(g1s) == 2);

	BOOST_CHECK(g1.get_accept().size() == 2);
}
