
#include "NFAGen.hpp"

std::unique_ptr<NFA> NFAGen::makeSimple(const std::string& regex)
{
	auto result = std::make_unique<NFA>();
	int first = result->make_vertex();

	result->set_start(first);

	for(size_t s = 0; s < regex.size(); s++)
	{
		int second = result->make_vertex();
		result->add_edge(first, second, NFAWeight(regex[s]));

		first = second;
	}

	result->add_accept(first);
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
	a.add_edge(n_start, prev, NFAWeight());

	a.merge_at(b, n_start);

	int n_accept = a.make_vertex();

	for(int vertex : a.get_accept())
		a.add_edge(vertex, n_accept, NFAWeight());

	a.clear_accept();
	a.add_accept(n_accept);
}

void NFAGen::makeStar(NFA &a)
{
	int n_start = a.make_vertex();
	int n_mid = a.make_vertex();

	int prev = a.get_start();
	a.set_start(n_start);

	a.add_edge(n_start, prev, NFAWeight());
	a.add_edge(n_start, n_mid, NFAWeight());

	int n_accept = a.make_vertex();

	a.add_edge(n_mid, prev, NFAWeight());
	a.add_edge(n_mid, n_accept, NFAWeight());

	for(int vertex : a.get_accept())
		a.add_edge(vertex, n_mid, NFAWeight());

	a.clear_accept();
	a.add_accept(n_accept);
}

