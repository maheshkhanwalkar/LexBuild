
#ifndef LEXBUILD_NFAGEN_HPP
#define LEXBUILD_NFAGEN_HPP

#include "FA/NFA.hpp"
#include <memory>

struct NFAGen
{
	/**
	 * Construct an NFA representing a simple regex
	 * @param regex - string of characters (no operators)
	 * @return equivalent NFA
	 */
	static std::unique_ptr<NFA> makeSimple(const std::string& regex);

	/**
	 * Construct an NFA representing: ab
	 *   Side-effect: [write-op] ab -> a
	 *
	 * @param a - one source NFA
	 * @param b - other source NFA
	 */
	static void makeAnd(NFA& a, NFA& b);

	/**
	 * Construct an NFA representing: a | b
	 *   Side-effect: [write-op] (a | b) -> a
	 *
	 * @param a - one source NFA
	 * @param b - other source NFA
	 */
	static void makeOr(NFA& a, NFA& b);

	/**
	 * Construct an NFA representing: a*
	 *   Side-effect: [write-op] a* -> a
	 *
	 * @param a - source NFA
	 */
	static void makeStar(NFA& a);

};


#endif
