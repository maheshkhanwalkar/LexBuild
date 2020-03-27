#pragma once

template <typename T, typename W>
edge<T, W>::edge(T start, T end, W weight)
    : start(start), end(end), weight(weight) { }

template<typename T, typename W>
const T& edge<T, W>::get_start() const {
    return start;
}

template<typename T, typename W>
const T& edge<T, W>::get_end() const {
    return end;
}

template<typename T, typename W>
const W& edge<T, W>::get_weight() const {
    return weight;
}

template<typename T, typename W>
bool edge<T, W>::operator==(const edge<T, W>& rhs) const
{
    return this->start == rhs.start && this->end == rhs.end &&
                this->weight == rhs.weight;
}
