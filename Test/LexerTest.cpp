
#include "Lexer/Lexer.hpp"

#define BOOST_TEST_MODULE LexerTest
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE(lexer_simple)
{
	Lexer<int> lexer;

	lexer.add_rule("abc", 2, [](std::string str) -> std::unique_ptr<int> 
	{
		return std::make_unique<int>(1);
	});
	
	lexer.add_rule(" ", 2, [](std::string str) -> std::unique_ptr<int> 
	{
		return std::make_unique<int>(2);
	});
	
	std::string str = "abc abc abc";
	std::vector<std::unique_ptr<int>> result = lexer.lex(str);
	
	BOOST_CHECK(result.size() == 5);
	
	for(size_t s = 0; s < result.size(); s++)
	{
		if(s % 2 == 0)
			BOOST_CHECK(*result[s] == 1);
		else
			BOOST_CHECK(*result[s] == 2);
	}
}

