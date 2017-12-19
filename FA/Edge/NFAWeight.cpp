
#include "NFAWeight.hpp"

NFAWeight::NFAWeight(char weight, WeightType type)
{
	this->weight = weight;
	this->type = type;
}

char NFAWeight::get_weight()
{
	return weight;
}

WeightType NFAWeight::get_type()
{
	return type;
}
