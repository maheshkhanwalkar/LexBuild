
#include <iostream>
#include "Graph/WeightedDigraph.tpp"

bool test_digraph()
{
	WeightedDigraph<int> test;

	for (int i = 0; i < 1000; i++)
		test.add_edge(i, i + 1, i * 2);

	for (int i = 0; i < 1000; i++)
	{
		if(test.has_edge(i + 1, i))
		{
			std::cerr << "Detected backwards edge" << std::endl;
			return false;
		}
	}

	for (int i = 1000; i > 0; i--)
	{
		std::shared_ptr<int> ptr = test.get_weight(i - 1, i);

		if(!ptr)
		{
			std::cerr << "Edge should exist" << std::endl;
			return false;
		}

		if(*ptr != (i - 1) * 2)
		{
			std::cerr << "Detected bad edge weight" << std::endl;
			return false;
		}
	}

	for (int i = 100; i < 350; i += 2)
		test.remove_edge(i, i + 1);

	for (int i = 100; i < 350; i += 2)
	{
		if(test.has_edge(i, i + 1))
		{
			std::cerr << "Edge shouldn't exist"	<< std::endl;
			return false;
		}
	}


	return true;
}

/* Main */
int main(int argc, const char* argv[])
{
	(void)argc;
	(void)argv;

	if(!test_digraph())
		return -1;

	std::cout << "All tests passed." << std::endl;
	return 0;
}
