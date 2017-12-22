
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
	 * @param priority - rule priority to break match ties (smaller is higher priority)
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

	regex_vec.push_back(RuleInfo(*result, on_accept, priority));
}

template<class T>
std::vector<std::unique_ptr<T>> Lexer<T>::lex(std::string& data)
{
	std::vector<std::unique_ptr<T>> result;

	/* TODO */

	return result;
}


#endif
