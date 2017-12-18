
#include "NFAGen.hpp"

std::unique_ptr<NFA> NFAGen::makeSimple(const std::string& regex)
{
	auto result = std::make_unique<NFA>(NFA(0));
	int count = 0;

	for(size_t s = 0; s < regex.size(); s++)
	{
		result->add_edge(count, count + 1, {regex[s], Edge_Type::NORMAL});
		count++;
	}

	result->add_accept(count);
	return result;
}

void NFAGen::makeAnd(NFA& a, NFA& b)
{
	return a.merge(b);
}


void NFAGen::makeOr(NFA& a, NFA& b)
{
	int n_start = a.make_vertex();
	int prev = a.get_start();

	a.set_start(n_start);
	a.add_edge(n_start, prev, {0, Edge_Type::EPSILON});

	a.merge_at(b, n_start);

	int n_accept = a.make_vertex();

	for(int vertex : a.get_accept())
		a.add_edge(vertex, n_accept, {0, Edge_Type::EPSILON});

	a.clear_accept();
	a.add_accept(n_accept);
}

void NFAGen::makeStar(NFA &a)
{
	int n_start = a.make_vertex();
	int n_mid = a.make_vertex();

	int prev = a.get_start();
	a.set_start(n_start);

	a.add_edge(n_start, prev, {0, Edge_Type::EPSILON});
	a.add_edge(n_start, n_mid, {0, Edge_Type::EPSILON});

	int n_accept = a.make_vertex();

	a.add_edge(n_mid, prev, {0, Edge_Type::EPSILON});
	a.add_edge(n_mid, n_accept, {0, Edge_Type::EPSILON});

	for(int vertex : a.get_accept())
		a.add_edge(vertex, n_mid, {0, Edge_Type::EPSILON});

	a.clear_accept();
	a.add_accept(n_accept);
}

