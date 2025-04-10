#include "GridAutomorphism.hpp"

GridRelation GridAutomorphism_GRID::apply(GridRelation R) {
    return GridRelation(R, f(R.G));
}; 

GridRelation GridAutomorphism_RELATION::apply(GridRelation R) {
    std::vector<IntVec> V;
    for (auto it = R.vars.begin(); it != R.vars.end(); it++) {
        V.push_back(f(*it));
    }
    return GridRelation(R, V);
}; 

GridAutomorphism::GridAutomorphism(std::function<Grid(Grid)> f_in) {
    is_grid = 1;
    G1 = GridAutomorphism_GRID(f_in);
};

GridAutomorphism::GridAutomorphism(std::function<IntVec(IntVec)> f_in) {
    is_grid = 0;
    G2 = GridAutomorphism_RELATION(f_in);
}; 

GridRelation GridAutomorphism::apply(GridRelation R) {
    if (is_grid) {
        return G1.apply(R);
    } else {
        return G2.apply(R);
    }
}; 
