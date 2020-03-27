#pragma once

#include <unordered_map>
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
public:
    /**
     * Construct an empty digraph
     */
    explicit digraph() = default;

private:
    std::unordered_multimap<T, edge<T, W> > adj_map{};
};
