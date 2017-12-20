
#ifndef LEXBUILD_RULEINFO_HPP
#define LEXBUILD_RULEINFO_HPP

#include <memory>

#include "FA/DFA.hpp"

template <class T>
class RuleInfo
{
public:
	/**
	 * Initialize RuleInfo
	 * @param dfa - DFA for the rule
	 * @param on_apply - function for the run
	 * @param priority - rule priority
	 */
	RuleInfo(DFA dfa,
		std::function<T(std::string)> on_apply, int priority);

	/**
	 * Get the DFA
	 * @return the stored DFA
	 */
	DFA get_dfa();

	/**
	 * Get the function
	 * @return the function
	 */
	std::function<T(std::string)> get_func();

	/**
	 * Get rule priority
	 * @return the priority
	 */
	int get_priority();

private:
	DFA dfa;
	std::function<T(std::string)> on_apply;
	int priority;
};

template<class T>
RuleInfo<T>::RuleInfo(DFA dfa, std::function<T(std::string)> on_apply, int priority)
{
	this->dfa = dfa;
	this->on_apply = on_apply;
	this->priority = priority;
}

template<class T>
DFA RuleInfo<T>::get_dfa()
{
	return dfa;
}

template<class T>
std::function<T(std::string)> RuleInfo<T>::get_func()
{
	return on_apply;
}

template<class T>
int RuleInfo<T>::get_priority()
{
	return priority;
}


#endif
