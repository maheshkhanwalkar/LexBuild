
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
	 * Initialize an epsilon NFA edge
	 */
	explicit NFAWeight();

	/**
	 * Initialize a normal NFA edge
	 * @param weight - edge weight
	 */
	explicit NFAWeight(char weight);

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

	/**
	 * Compare two NFAWeight objects
	 * @param other - object to compare with
	 * @return true, if they are equal
	 */
	bool operator==(const NFAWeight& other) const;

private:
	char weight;
	WeightType type;
};


#endif
