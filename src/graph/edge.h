#pragma once

/**
 * Edge representation
 * @tparam T - vertex type
 * @tparam W - weight type
 */
template <typename T, typename W>
class edge
{
public:
    /**
     * Initialise an edge
     * @param start - starting vertex
     * @param end - ending vertex
     * @param weight - edge weight
     */
    explicit edge(T start, T end, W weight);

    /**
     * Get the start vertex
     * @return the starting vertex
     */
    const T& get_start() const;

    /**
     * Get the ending vertex
     * @return the ending vertex
     */
    const T& get_end() const;

    /**
     * Get the edge weight
     * @return the weight
     */
    const W& get_weight() const;

private:
    T start, end;
    W weight;
};

#include "edge.tpp"
