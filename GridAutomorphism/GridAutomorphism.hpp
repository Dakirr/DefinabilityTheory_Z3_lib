#ifndef GRID_AUTOMORHPISM_HPP
#define GRID_AUTOMORHPISM_HPP

#include "../GridRelation/GridRelation.hpp"

class GridAutomorphism_GRID {
    public:
    std::function<Grid(Grid)> f;
    GridAutomorphism_GRID(std::function<Grid(Grid)> f_in): f(f_in) {};
    GridAutomorphism_GRID(){};
    GridRelation apply(GridRelation R); 
};

class GridAutomorphism_RELATION {
    public:
    std::function<IntVec(IntVec)> f;
    GridAutomorphism_RELATION(std::function<IntVec(IntVec)> f_in): f(f_in) {};
    GridAutomorphism_RELATION(){};
    GridRelation apply(GridRelation R); 
};

class GridAutomorphism {
    public:
    bool is_grid;
    GridAutomorphism_GRID G1;
    GridAutomorphism_RELATION G2;
    GridAutomorphism(std::function<Grid(Grid)> f_in);
    GridAutomorphism(std::function<IntVec(IntVec)> f_in); 
    GridRelation apply(GridRelation R);    
};

#endif