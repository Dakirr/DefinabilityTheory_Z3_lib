#ifndef GRID_FUNCTION_HPP
#define GRID_FUNCTION_HPP

#include "Grid.hpp"

template <typename T1>
class GridFunction {
    std::function<z3::expr(T1, Grid)> f;
    public:
    GridFunction(std::function<z3::expr(T1, Grid)> f_in): f(f_in) {};
    std::function<z3::expr(T1)> get_f(Grid G);
};




#endif