#include "GridRelation.hpp"

z3::expr GridRelation::get_relation() {
    return gf.get_f(G)(vars);
};
z3::expr GridRelation::get_relation(std::vector<IntVec> vars_in) {
    gf.get_f(G)(vars_in);
};
z3::expr GridRelation::get_relation(std::vector<int> indexes) {
    std::vector<IntVec> vars1(vars);
    for (int it = 0; it < indexes.size(); it++) {
        vars1[it] = vars[indexes[it]]; 
    }
    return get_relation(vars1);
};
z3::expr GridRelation::get_relation(z3::func_decl f1, z3::func_decl f2) {
    std::vector<IntVec> vars1;
    for (auto i = vars.begin(); i != vars.end(); ++i) {
        vars1.push_back(IntVec(f1(i->first), f2(i->second)));
    }
    return get_relation(vars1); 
}

GridRelation GridRelation::operator||(GridRelation other) {
    return GridRelation(GridFunction<std::vector<IntVec>>(
                        [this, other](std::vector<IntVec> vars, Grid G1){
                            GridFunction A = GridFunction(other.gf);    
                            return (this->gf.get_f(G1)(vars)) || (A.get_f(G1)(vars));
                        }), vars, G);
};

GridRelation GridRelation::operator&&(GridRelation other) {
    return GridRelation(GridFunction<std::vector<IntVec>>(
                        [this, other](std::vector<IntVec> vars, Grid G1){
                            GridFunction A = GridFunction(other.gf);    
                            return (this->gf.get_f(G1)(vars)) && (A.get_f(G1)(vars));
                        }), vars, G);
};

GridRelation GridRelation::operator!() {
    return GridRelation(GridFunction<std::vector<IntVec>>(
        [this](std::vector<IntVec> vars, Grid G1){
            return !(this->gf.get_f(G1)(vars));
        }), vars, G);
};