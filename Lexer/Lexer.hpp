
#ifndef LEXBUILD_LEXER_HPP
#define LEXBUILD_LEXER_HPP

#include <string>
#include <functional>
#include <unordered_map>
#include <iostream>

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
	void add_rule(std::string regex, int priority,
				  std::function<void(std::string, std::vector<std::unique_ptr<T>>&)> on_accept);

	/**
	 * Lexically analyze the data/file
	 * @param data - data to process
	 * @param result - vector to store resulting tokens
	 * @return if lexing was successful
	 */
	bool lex(std::string& data, std::vector<std::unique_ptr<T>>& result);

private:

	/* Map from regex string to the RuleInfo */
	std::vector<RuleInfo<T>> regex_vec;
};

template<class T>
void Lexer<T>::add_rule(std::string regex, int priority,
		std::function<void(std::string, std::vector<std::unique_ptr<T>>&)> on_accept)
{
	Builder builder(regex);

	std::unique_ptr<Regex> symbolic = builder.create();
	std::unique_ptr<DFA> result = symbolic->generate().transform();

	regex_vec.push_back(RuleInfo<T>(std::move(result), on_accept, priority));
}

template<class T>
bool Lexer<T>::lex(std::string& data, std::vector<std::unique_ptr<T>>& result)
{
	for(size_t s = 0; s < data.size(); s++)
	{
		std::vector<size_t> can_accept;
		size_t max_len = 0;

		bool moved = false;

		for(size_t u = 0; u < regex_vec.size(); u++)
		{
			DFA& dfa = regex_vec[u].get_dfa();

			if(dfa.peek(data[s]))
			{
				dfa.consume(data[s]);
				moved = true;
			}
			else
			{
				dfa.reset();
			}

			/* At accept? */
			if(dfa.at_accept())
			{
				can_accept.push_back(u);

				if(dfa.get_data()->size() > max_len)
					max_len = dfa.get_data()->size();
			}
		}

		if(can_accept.empty())
		{
			if(!moved)
				return false;

			continue;
		}

		std::vector<size_t> shift;

		for(size_t u = 0; u < regex_vec.size(); u++)
		{
			if(s + 1 < data.size())
			{
				DFA& dfa = regex_vec[u].get_dfa();

				/* Only shift if we can match more characters */
				if(dfa.peek(data[s+1]) && dfa.get_data()->size() + 1 > max_len)
					shift.push_back(u);
			}
		}

		int priority = INT32_MIN;
		size_t pos = 0;

		/* Break tie based on priority */
		if(shift.empty())
		{
			for(size_t u = 0; u < can_accept.size(); u++)
			{
				int curr = regex_vec[can_accept[u]].get_priority();

				if(curr > priority)
				{
					priority = curr;
					pos = can_accept[u];
				}
			}

			RuleInfo<T>& info = regex_vec[pos];
			auto f_apply = info.get_func();

			f_apply(*info.get_dfa().get_data(), result);

			/* Reset everything */
			for(size_t u = 0; u < regex_vec.size(); u++)
				regex_vec[u].get_dfa().reset();
		}
		else
		{
			/* Reset accept DFAs that can't shift */
			for(size_t u = 0; u < can_accept.size(); u++)
			{
				DFA& dfa = regex_vec[can_accept[u]].get_dfa();

				/* Reset accept DFAs that can't shift */
				if(!dfa.peek(data[s+1]))
				{
					regex_vec[can_accept[u]].get_dfa().reset();
				}
			}
		}
	}

	return true;
}

#endif
