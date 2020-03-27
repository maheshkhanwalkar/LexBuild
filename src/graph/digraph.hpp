#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

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
        weight_map[start][weight] = end;
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
    std::unordered_map<T, std::unordered_map<W, T> > weight_map{};
};
