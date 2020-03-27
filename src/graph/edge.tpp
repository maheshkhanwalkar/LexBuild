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
