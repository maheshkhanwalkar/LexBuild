
#define BOOST_TEST_MODULE BuilderTest
#include <boost/test/included/unit_test.hpp>

#include "Regex/Builder.hpp"

void check_simple(const Regex* result, std::string data)
{
	BOOST_CHECK(result->get_type() == RegexType::SIMPLE);
	auto equiv = dynamic_cast<const SimpleRegex*>(result);

	BOOST_CHECK(equiv);
	BOOST_CHECK(equiv->get_string() == data);
}

BOOST_AUTO_TEST_CASE(builder_test_simple)
{
	Builder builder("abcdef");
	std::unique_ptr<Regex> result = builder.create();

	check_simple(result.get(), "abcdef");
}

BOOST_AUTO_TEST_CASE(builder_test_paren)
{
	Builder builder("(abc)");
	std::unique_ptr<Regex> r1 = builder.create();

	check_simple(r1.get(), "abc");

	builder = Builder("(abc)(def)");
	std::unique_ptr<Regex> r2 = builder.create();

	check_simple(r2.get(), "abcdef");
}

BOOST_AUTO_TEST_CASE(builder_test_or)
{
	Builder builder("(abc)|(def)");
	std::unique_ptr<Regex> r1 = builder.create();

	BOOST_CHECK(r1->get_type() == RegexType::OR);
	OrRegex* equiv = dynamic_cast<OrRegex*>(r1.get());

	check_simple(&equiv->get_lhs(), "abc");
	check_simple(&equiv->get_rhs(), "def");
}

BOOST_AUTO_TEST_CASE(builder_test_star_split)
{
	Builder builder("abc*");
	std::unique_ptr<Regex> r1 = builder.create();

	BOOST_CHECK(r1->get_type() == RegexType::AND);
	AndRegex* equiv = dynamic_cast<AndRegex*>(r1.get());

	check_simple(&equiv->get_lhs(), "ab");

	BOOST_CHECK(equiv->get_rhs().get_type() == RegexType::STAR);
	auto rhs = dynamic_cast<const StarRegex*>(&equiv->get_rhs());

	check_simple(&rhs->get_target(), "c");
}
