
#ifndef LEXBUILD_LEXER_HPP
#define LEXBUILD_LEXER_HPP

#include <string>
#include <functional>
#include <unordered_map>

#include "FA/DFA.hpp"
#include "Lexer/RuleInfo.hpp"
#include "Regex/Builder.hpp"

/**
 * (Public) Lexer API
 * @tparam T - token type
 */
template <class T>
class Lexer
{
public:
	/**
	 * Add a rule to the Lexer
	 * @param regex - regex to match
	 * @param priority - rule priority to break match ties (larger is higher priority)
	 * @param on_accept - function to apply when the regex is matched
	 */
	void add_rule(std::string regex, int priority, std::function<std::unique_ptr<T>(std::string)> on_accept);

	/**
	 * Lexically analyze the data/file
	 * @param data - data to process
	 * @return a vector containing the results of lexical analysis (tokens)
	 */
	std::vector<std::unique_ptr<T>> lex(std::string& data);

private:

	/* Map from regex string to the RuleInfo */
	std::vector<RuleInfo<T>> regex_vec;
};

template<class T>
void Lexer<T>::add_rule(std::string regex, int priority, std::function<std::unique_ptr<T>(std::string)> on_accept)
{
	Builder builder(regex);

	std::unique_ptr<Regex> symbolic = builder.create();
	std::unique_ptr<DFA> result = symbolic->generate().transform();

	regex_vec.push_back(RuleInfo(std::move(result), on_accept, priority));
}

template<class T>
std::vector<std::unique_ptr<T>> Lexer<T>::lex(std::string& data)
{
	std::vector<std::unique_ptr<T>> result;

	for(size_t s = 0; s < data.size(); s++)
	{
		std::vector<size_t> can_accept;

		for(size_t u = 0; u < regex_vec.size(); u++)
		{
			DFA& dfa = regex_vec[u].get_dfa();

			if(dfa.peek(data[s]))
				dfa.consume(data[s]);

			/* At accept? */
			if(dfa.at_accept())
				can_accept.push_back(u);
		}

		std::vector<size_t> shift;

		for(size_t u = 0; u < can_accept.size(); u++)
		{
			if(s + 1 < data.size())
			{
				DFA& dfa = regex_vec[can_accept[u]].get_dfa();

				if(dfa.peek(data[s+1]))
					shift.push_back(can_accept[u]);
			}
		}

		int priority = INT32_MIN;
		size_t pos = 0;

		/* Break tie based on priority */
		if(shift.empty())
		{
			for(size_t u = 0; u < shift.size(); u++)
			{
				int curr = regex_vec[shift[u]].get_priority();

				if(curr > priority)
				{
					priority = curr;
					pos = shift[u];
				}
			}
		}

		RuleInfo info = regex_vec[pos];
		auto f_apply = info.get_func();

		result.push_back(f_apply(*info.get_dfa().get_data()));
	}

	return result;
}


#endif
