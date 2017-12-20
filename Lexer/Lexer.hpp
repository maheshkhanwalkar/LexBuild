
#ifndef LEXBUILD_LEXER_HPP
#define LEXBUILD_LEXER_HPP

#include <string>
#include <functional>
#include <unordered_map>

#include "FA/DFA.hpp"
#include "Lexer/RuleInfo.hpp"

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
	 * @return true, if successfully added
	 */
	bool add_rule(std::string regex, int priority, std::function<T(std::string)> on_accept);

	/**
	 * Remove a rule from the Lexer
	 * @param regex - regex rule to remove
	 * @return true, if successfully removed
	 */
	bool remove_rule(std::string regex);

	/**
	 * Lexically analyze the data/file
	 * @param data - data to process
	 * @return a vector containing the results of lexical analysis (tokens)
	 */
	std::vector<T> lex(std::string& data);

private:

	/* Map from regex string to the RuleInfo */
	std::unordered_map<std::string, RuleInfo<T>> regex_map;
};

template<class T>
bool Lexer<T>::add_rule(std::string regex, int priority, std::function<T(std::string)> on_accept)
{
	/* Already exists */
	if(regex_map.find(regex) != regex_map.end())
		return false;

	/* TODO */

	return true;
}

template<class T>
bool Lexer<T>::remove_rule(std::string regex)
{
	const auto& itr = regex_map.find(regex);

	if(itr == regex_map.end())
		return false;

	regex_map.erase(itr);
	return true;
}


#endif
