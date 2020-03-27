#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "edge.hpp"

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

    /**
     * Add start---weight--->end to the digraph
     * @param start - starting vertex
     * @param end - ending vertex
     * @param weight - edge weight
     */
    void add_edge(T start, T end, W weight)
    {
        adj_map[start].insert(end);
        weight_map[start][end] = weight;
    }

    /**
     * Update the edge start-->end with a new weight
     * @param start - starting vertex
     * @param end - ending vertex
     * @param n_weight - new edge weight
     */
    void update_edge(T start, T end, W n_weight)
    {
        std::unordered_set<T>& out = adj_map[start];

        // Edge does not even exist
        if(out.find(end) == out.end()) {
            return;
        }

        // Update the weight in the weight map
        weight_map[start][end] = n_weight;
    }

    /**
     * Check whether there is an edge from start-->end
     * @param start - starting vertex
     * @param end - ending vertex
     * @return true, if there is such an edge
     */
    bool has_edge(T start, T end) const
    {
        const std::unordered_set<T>& out = adj_map.at(start);
        return out.find(end) != out.end();
    }

private:
    std::unordered_map<T, std::unordered_set<T> > adj_map{};
    std::unordered_map<T, std::unordered_map<T, W> > weight_map{};
};
