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
        weight_map[start].insert(std::make_pair(weight, end));
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

    /**
     * Perform a walk of length 1 starting from the given vertex, traversing all
     * immediate edges with the given weight
     * @param start - starting vertex
     * @param weight - edge weight to use
     * @return all the vertices reached by the walk
     */
    std::unordered_set<T> walk(T start, W weight) const
    {
        const std::unordered_multimap<W, T>& possible = weight_map.at(start);
        const auto& range = possible.equal_range(weight);

        std::unordered_set<T> reachable;

        for(auto itr = range.first; itr != range.second; itr++) {
            reachable.insert(itr->second);
        }

        return reachable;
    }

    void rename_vertices(const std::unordered_map<T, T>& mapping)
    {
        std::unordered_map<T, std::unordered_set<T> > adj_copy;

        // Update the adjacency mapping
        for(auto& elem : adj_map) {
            const T& n_elem = mapping.at(elem.first);
            adj_copy[n_elem] = rename_set(elem.second, mapping);
        }

        std::unordered_map<T, std::unordered_multimap<W, T> > weight_copy;

        // Update the weight mapping
        for(auto& elem : weight_map) {
            const T& n_elem = mapping.at(elem.first);

            for(auto& edge : elem.second) {
                edge.second = mapping.at(edge.second);
            }

            weight_copy[n_elem] = elem.second;
        }

        // Update the internal structures
        this->adj_map = adj_copy;
        this->weight_map = weight_copy;
    }

private:
    std::unordered_map<T, std::unordered_set<T> > adj_map{};
    std::unordered_map<T, std::unordered_multimap<W, T> > weight_map{};

    /**
     * Rename the elements in a set, using the provided mapping scheme
     * @param orig - original set
     * @param mapping - element-to-element renaming scheme
     * @return a new set containing the renamed elements
     */
    std::unordered_set<T> rename_set(const std::unordered_set<T>& orig,
            const std::unordered_map<T, T>& mapping)
    {
        std::unordered_set<T> result;

        // Element-by-element translation
        for(T elem : orig) {
            result.insert(mapping.at(elem));
        }

        return result;
    }
};
