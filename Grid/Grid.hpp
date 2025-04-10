#ifndef GRID_HPP
#define GRID_HPP
#include "IntVec/IntVec.hpp"

class Grid {
    public:
    IntVec x;
    IntVec y;
    Grid(IntVec x_in, IntVec y_in): x(x_in), y(y_in) {};
    Grid(): x(IntVec(c.int_val(1), c.int_val(0))), y(IntVec(c.int_val(0), c.int_val(1))) {};
};

#endif