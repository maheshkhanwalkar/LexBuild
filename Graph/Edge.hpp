
#ifndef LEXBUILD_EDGE_HPP
#define LEXBUILD_EDGE_HPP

template <class T, class K>
class Edge
{
public:
	/**
	 * Construct a new Edge
	 * @param src - source vertex
	 * @param dest - destination vertex
	 * @param weight - edge weight
	 * @param data - extra edge information
	 */
	Edge(int src, int dest, T weight, K data);

	/**
	 * Get source vertex
	 * @return vertex id of src
	 */
	int get_src();

	/**
	 * Get destination vertex
	 * @return vertex id of dest
	 */
	int get_dest();


	/**
	 * Get weight
	 * @return edge weight
	 */
	T get_weight();

	/**
	 * Get extra data
	 * @return stored data
	 */
	K get_data();

private:
	int src, dest;
	T weight;

	K data;
};

template <class T, class K>
Edge<T, K>::Edge(int src, int dest, T weight, K data)
{
	this->src = src;
	this->dest = dest;

	this->weight = weight;
	this->data = data;
}

template<class T, class K>
int Edge<T, K>::get_src()
{
	return src;
}

template<class T, class K>
int Edge<T, K>::get_dest()
{
	return dest;
}

template<class T, class K>
T Edge<T, K>::get_weight()
{
	return weight;
}

template<class T, class K>
K Edge<T, K>::get_data()
{
	return data;
}


#endif
