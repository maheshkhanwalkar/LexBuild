#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "edge.h"

/**
 * Digraph representation
 * @tparam T - vertex type
 * @tparam W - weight type
 */
template <typename T, typename W>
class digraph
{
private:
    std::unordered_map<T, std::vector<edge<T, W> > > adj_map{};
};

