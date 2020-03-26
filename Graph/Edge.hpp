
#ifndef LEXBUILD_EDGE_HPP
#define LEXBUILD_EDGE_HPP

#include <functional>

template <class T>
class Edge
{
public:
	/**
	 * Construct a new Edge
	 * @param src - source vertex
	 * @param dest - destination vertex
	 * @param weight - edge weight
	 */
	Edge(int src, int dest, T weight);

	/**
	 * Get source vertex
	 * @return vertex id of src
	 */
	int get_src() const;

	/**
	 * Get destination vertex
	 * @return vertex id of dest
	 */
	int get_dest() const;


	/**
	 * Get weight
	 * @return edge weight
	 */
	T get_weight() const;

	/**
	 * Compare edges
	 * @param rhs - edge to compare with
	 * @return true if the edges are the same
	 */
	bool operator==(const Edge& rhs) const;

private:
	int src, dest;
	T weight;
};

template <class T>
class EdgeHash
{
public:
	size_t operator() (const Edge<T>& edge) const
	{
		size_t seed = 0;

		hash_combine(seed, edge.get_src());
		hash_combine(seed, edge.get_dest());

		return seed;
	}

private:
	inline void hash_combine(size_t& seed, int next) const
	{
		std::hash<int> h_func;
		seed ^= h_func(next) + 0x9e3779b9 + (seed << 6U) + (seed >> 2U);
	}
};

template <class T>
Edge<T>::Edge(int src, int dest, T weight)
    : src(src), dest(dest), weight(weight) { }

template<class T>
int Edge<T>::get_src() const
{
	return src;
}

template<class T>
int Edge<T>::get_dest() const
{
	return dest;
}

template<class T>
T Edge<T>::get_weight() const
{
	return weight;
}

template<class T>
bool Edge<T>::operator==(const Edge<T>& rhs) const
{
	return this->src == rhs.src && this->dest == rhs.dest
			&& this->weight == rhs.weight;
}

#endif
