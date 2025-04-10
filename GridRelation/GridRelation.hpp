#ifndef GRID_RELATION_HPP
#define GRID_RELATION_HPP

#include "../Grid/GridFunction.hpp"

class GridRelation {
    public:
    GridFunction<std::vector<IntVec>> gf;
    std::vector<IntVec> vars;
    Grid G;

    GridRelation(GridFunction<std::vector<IntVec>> gf_in, std::vector<IntVec> vars_in, Grid G_in = Grid()): gf(gf_in), vars(vars_in), G(G_in) {};
    GridRelation(GridRelation other, std::vector<IntVec> v, Grid G_in): gf(other.gf), vars(v), G(G_in) {};
    GridRelation(GridRelation other, std::vector<IntVec> v): gf(other.gf), vars(v), G(other.G) {};
    GridRelation(GridRelation other, Grid G_in): gf(other.gf), vars(other.vars), G(G_in) {};
    GridRelation(GridRelation other, std::vector<int> indexes);
    
    z3::expr get_relation();
    z3::expr get_relation(std::vector<IntVec> vars);
    z3::expr get_relation(z3::func_decl f1, z3::func_decl f2);
    z3::expr get_relation(std::vector<int> indexes);
    GridRelation operator||(GridRelation other);
    GridRelation operator&&(GridRelation other);
    GridRelation operator!();
};

#endif
