
#ifndef LEXBUILD_BUILDER_HPP
#define LEXBUILD_BUILDER_HPP

#include "Regex/Regex.hpp"
#include <string>

class Builder
{
public:
	/**
	 * Create a Regex builder
	 * @param regex - string to build upon
	 */
	explicit Builder(std::string regex);

	/**
	 * Create a symbolic Regex representation
	 * @return the regular expression
	 */
	const Regex& create();

private:
	std::string regex;
	std::unique_ptr<Regex> result;

	/* Regex parsing helper functions */
	std::unique_ptr<Regex> parse_regex(size_t& pos);
	std::unique_ptr<Regex> parse_term(size_t& pos);
	std::unique_ptr<Regex> parse_factor(size_t& pos);
	std::unique_ptr<Regex> parse_base(size_t& pos);

	/* Is 'c' an operator? */
	bool is_op(char c);
};


#endif
