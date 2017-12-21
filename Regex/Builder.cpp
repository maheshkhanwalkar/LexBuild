
#include "Builder.hpp"
#include <stack>

Builder::Builder(std::string regex)
{
	this->regex = regex;
}

std::unique_ptr<Regex> Builder::create()
{
	size_t pos = 0;
	return parse_regex(pos);
}

std::unique_ptr<Regex> Builder::parse_regex(size_t& pos)
{
	std::unique_ptr<Regex> term = parse_term(pos);

	if(pos < regex.size())
	{
		if(regex[pos] == '|')
		{
			std::unique_ptr<Regex> rhs = parse_regex(++pos);
			std::unique_ptr<Regex> result(new OrRegex(std::move(term), std::move(rhs)));

			return result;
		}
	}

	return term;
}

std::unique_ptr<Regex> Builder::parse_term(size_t& pos)
{
	/* Empty */
	if(pos >= regex.size())
		return std::unique_ptr<Regex>(new SimpleRegex(""));

	std::unique_ptr<Regex> result = nullptr;

	while(pos < regex.size())
	{
		if(!result)
		{
			result = parse_factor(pos);
		}
		else
		{
			/* No more factors */
			if(regex[pos] == ')' || regex[pos] == '|')
				return result;

			std::unique_ptr<Regex> rhs = parse_factor(pos);
			std::unique_ptr<Regex> combined(new AndRegex(std::move(result), std::move(rhs)));

			result = std::move(combined);
		}
	}

	return result;
}

std::unique_ptr<Regex> Builder::parse_factor(size_t& pos)
{
	std::unique_ptr<Regex> base = parse_base(pos);

	if(pos >= regex.size())
		return base;

	if(regex[pos] != '*')
		return base;

	/* Consume all the stars */
	while(regex[pos] == '*' && pos < regex.size())
		pos++;

	std::unique_ptr<Regex> result(new StarRegex(std::move(base)));
	return result;
}

std::unique_ptr<Regex> Builder::parse_base(size_t& pos)
{
	if (regex[pos] == '(')
	{
		std::unique_ptr<Regex> result = parse_regex(++pos);
		pos++;

		return result;
	}
	else
	{
		/* Consume all the normal characters */
		std::string full;

		while(!is_op(regex[pos]) && pos < regex.size())
		{
			full += std::string(1, regex[pos]);
			pos++;
		}

		return std::unique_ptr<Regex>(new SimpleRegex(full));
	}

}

bool Builder::is_op(char c)
{
	return c == '(' || c == ')' || c == '|' || c == '*';
}
