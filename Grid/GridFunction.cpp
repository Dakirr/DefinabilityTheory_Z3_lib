#include "GridFunction.hpp"
template <typename T1>
std::function<z3::expr(T1)> GridFunction<T1>::get_f(Grid G) {
    return [this, G](T1 T1_ex){return this->f(T1_ex, G);};
};
template <>
std::function<z3::expr(std::vector<IntVec>)> GridFunction<std::vector<IntVec>>::get_f(Grid G) {
    return [this, G](std::vector<IntVec> T1_ex){return this->f(T1_ex, G);};
};