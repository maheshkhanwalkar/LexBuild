
#include "Lexer/Lexer.hpp"

#define BOOST_TEST_MODULE LexerTest
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE(lexer_simple)
{
	Lexer<int> lexer;

	lexer.add_rule("abc", 2, [](std::string str,
								std::vector<std::unique_ptr<int>>& vec) -> void
	{
		(void)str;
		vec.push_back(std::make_unique<int>(1));
	});

	lexer.add_rule(" ", 2, [](std::string str,
								std::vector<std::unique_ptr<int>>& vec) -> void
	{
		/* Do nothing */

		(void)str;
		(void)vec;
	});

	std::string str = "abc abc abc";
	std::vector<std::unique_ptr<int>> result;

	lexer.lex(str, result);

	BOOST_CHECK(result.size() == 3);

	for(size_t s = 0; s < result.size(); s++)
	{
		BOOST_CHECK(*result[s] == 1);
	}
}

