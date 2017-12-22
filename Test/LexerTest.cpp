
#include "Lexer/Lexer.hpp"

#define BOOST_TEST_MODULE LexerTest
#include <boost/test/included/unit_test.hpp>

#define UNUSED(x) (void)x

BOOST_AUTO_TEST_CASE(lexer_simple)
{
	Lexer<int> lexer;

	lexer.add_rule("abc", 2, [](std::string str,
								std::vector<std::unique_ptr<int>>& vec) -> void
	{
		UNUSED(str);
		vec.push_back(std::make_unique<int>(1));
	});

	/* Do nothing (e.g. discard whitespace) */
	lexer.add_rule(" |\t|\n", 2, [](std::string str,
								std::vector<std::unique_ptr<int>>& vec) -> void
	{
		UNUSED(str);
		UNUSED(vec);
	});

	std::string str = "abc   \tabc abc\n\nabc";
	std::vector<std::unique_ptr<int>> result;

	BOOST_CHECK(lexer.lex(str, result));
	BOOST_CHECK(result.size() == 4);

	for(size_t s = 0; s < result.size(); s++)
		BOOST_CHECK(*result[s] == 1);
}

BOOST_AUTO_TEST_CASE(lexer_shift_rule)
{
	Lexer<int> lexer;

	lexer.add_rule("int", 10, [](std::string str,
								std::vector<std::unique_ptr<int>>& vec) -> void
	{
		UNUSED(str);
		vec.push_back(std::make_unique<int>(1));
	});

	lexer.add_rule("intabc", 2, [](std::string str,
								std::vector<std::unique_ptr<int>>& vec) -> void
	{
		UNUSED(str);
		vec.push_back(std::make_unique<int>(2));
	});

	/* Do nothing (e.g. discard whitespace) */
	lexer.add_rule(" |\t|\n", 1, [](std::string str,
									std::vector<std::unique_ptr<int>>& vec) -> void
	{
		UNUSED(str);
		UNUSED(vec);
	});

	std::string str = "int intabc";
	std::vector<std::unique_ptr<int>> result;

	BOOST_CHECK(lexer.lex(str, result));
	BOOST_CHECK(result.size() == 2);

	BOOST_CHECK(*result[0] == 1);
	BOOST_CHECK(*result[1] == 2);
}

BOOST_AUTO_TEST_CASE(lexer_fallthrough)
{
	Lexer<int> lexer;

	lexer.add_rule("abc", 10, [](std::string str,
								 std::vector<std::unique_ptr<int>>& vec) -> void
	{
		UNUSED(str);
		vec.push_back(std::make_unique<int>(1));
	});

	std::string str = "abz";
	std::vector<std::unique_ptr<int>> result;

	BOOST_CHECK(!lexer.lex(str, result));
	BOOST_CHECK(result.size() == 0);
}

