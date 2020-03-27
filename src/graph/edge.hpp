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
    explicit edge(T start, T end, W weight)
            : start(start), end(end), weight(weight) {}

    /**
     * Get the start vertex
     * @return the starting vertex
     */
    const T& get_start() const {
        return start;
    }

    /**
     * Get the ending vertex
     * @return the ending vertex
     */
    const T& get_end() const {
        return end;
    }

    /**
     * Get the edge weight
     * @return the weight
     */
    const W& get_weight() const {
        return weight;
    }

    /**
     * Update the edge weight
     * @param n_weight - new weight
     */
    void update_weight(W n_weight) {
        this->weight = n_weight;
    }

    /**
     * Equality operator
     * @param rhs - other edge to compare to
     * @return true, if the edges are the same, false otherwise
     */
    bool operator==(const edge<T,W>& rhs) const
    {
        return this->start == rhs.start && this->end == rhs.end &&
               this->weight == rhs.weight;
    }

private:
    T start, end;
    W weight;
};
