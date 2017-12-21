
#ifndef LEXBUILD_REGEX_HPP
#define LEXBUILD_REGEX_HPP

#include "FA/NFA.hpp"
#include "Gen/NFAGen.hpp"

enum class RegexType
{
	SIMPLE, OR, AND, STAR
};

class Regex
{
public:
	/**
	 * Create a generic regex
	 * @param type - regex type
	 */
	explicit Regex(RegexType type)
	{
		this->type = type;
	}

	/**
	 * Get the regex type
	 * @return the type
	 */
	RegexType get_type() const
	{
		return type;
	}

	/**
	 * Generate an equivalent NFA
	 * @return the NFA
	 */
	virtual NFA generate() const = 0;

private:
	RegexType type;
};

class SimpleRegex : public Regex
{
public:
	explicit SimpleRegex(std::string data) : Regex(RegexType::SIMPLE)
	{
		this->data = data;
	}

	std::string get_string() const
	{
		return data;
	}

	NFA generate() const override
	{
		/* TODO rethink the NFAGen semantics */
		NFA copy = *(NFAGen::makeSimple(data));
		return copy;
	}

private:
	std::string data;
};


class OrRegex : public Regex
{
public:
	explicit OrRegex(std::unique_ptr<Regex> lhs, std::unique_ptr<Regex> rhs)
		: Regex(RegexType::OR), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

	const Regex& get_lhs() const
	{
		return *lhs;
	}

	const Regex& get_rhs() const
	{
		return *rhs;
	}

	NFA generate() const override
	{
		NFA left = lhs->generate();
		NFA right = rhs->generate();

		NFAGen::makeOr(left, right);
		return left;
	}

private:
	std::unique_ptr<Regex> lhs;
	std::unique_ptr<Regex> rhs;
};

class AndRegex : public Regex
{
public:
	explicit AndRegex(std::unique_ptr<Regex> lhs, std::unique_ptr<Regex> rhs)
		: Regex(RegexType::AND), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

	const Regex& get_lhs() const
	{
		return *lhs;
	}

	const Regex& get_rhs() const
	{
		return *rhs;
	}

	NFA generate() const override
	{
		NFA left = lhs->generate();
		NFA right = rhs->generate();

		NFAGen::makeAnd(left, right);
		return left;
	}

private:
	std::unique_ptr<Regex> lhs;
	std::unique_ptr<Regex> rhs;
};

class StarRegex : public Regex
{
public:
	explicit StarRegex(std::unique_ptr<Regex> src)
		: Regex(RegexType::STAR), target(std::move(src)) {}

	const Regex& get_target() const
	{
		return *target;
	}

	NFA generate() const override
	{
		NFA base = target->generate();
		NFAGen::makeStar(base);

		return base;
	}

private:
	std::unique_ptr<Regex> target;
};

#endif
