
#include "NFAWeight.hpp"

NFAWeight::NFAWeight()
{
	this->type = WeightType::EPSILON;
	this->weight = 0;
}

NFAWeight::NFAWeight(WeightType type, char weight)
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

bool NFAWeight::operator==(const NFAWeight& other) const
{
	return this->weight == other.weight
		   && this->type == other.type;
}
