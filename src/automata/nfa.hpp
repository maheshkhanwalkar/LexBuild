#pragma once
#include <string>

#include "src/graph/digraph.hpp"

class NFA
{
public:
    /**
     * Construct an empty NFA
     */
    explicit NFA() = default;

    /**
     * Construct an NFA which accepts a particular word
     * @param word - word to accept
     * @return the newly constructed NFA
     */
    static NFA make_simple(const std::string& word);

private:
    digraph<int, std::string> graph{};
    std::unordered_set<int> accept{};
    int start{-1};
};
