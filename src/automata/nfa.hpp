#pragma once

#include <string>
#include <unordered_set>
#include <vector>

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
    std::vector<int> v_list{};

    int start{-1}, v_count{0};
};
