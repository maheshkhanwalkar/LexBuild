
#ifndef LEXBUILD_NFAEDGE_HPP
#define LEXBUILD_NFAEDGE_HPP

enum class WeightType
{
	NORMAL, EPSILON
};

class NFAWeight
{
public:
	/**
	 * Initialize an NFA edge
	 * @param weight - edge weight
	 * @param type - type of edge
	 */
	NFAWeight(char weight, WeightType type);

	/**
	 * Get NFA edge transition character
	 * @return the character
	 */
	char get_weight();

	/**
	 * Get NFA edge transition type
	 * @return the type
	 */
	WeightType get_type();

private:
	char weight;
	WeightType type;
};


#endif
