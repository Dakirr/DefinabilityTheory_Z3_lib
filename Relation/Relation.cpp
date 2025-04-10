#include "Relation.hpp"

Relation::Relation(Relation other, std::vector<int> indexes) {
    std::vector<z3::expr> vars1(vars);
    for (int it = 0; it < indexes.size(); it++) {
        vars1[it] = vars[indexes[it]]; 
    }
    Relation(other.f, vars1);
}

z3::expr Relation::get_relation() {
    return f(vars);
}

z3::expr Relation::get_relation(std::vector<z3::expr> v) {
    return f(v);
}

z3::expr Relation::get_relation(std::vector<int> indexes) {
    std::vector<z3::expr> vars1(vars);
    for (int it = 0; it < indexes.size(); it++) {
        vars1[it] = vars[indexes[it]]; 
    }
    return get_relation(vars1);
};

z3::expr Relation::get_relation(z3::func_decl f1) {
    std::vector<z3::expr> vars1;
    for (auto i = vars.begin(); i != vars.end(); ++i) {
        vars1.push_back(f1(*i));
    }
    return f(vars1); 
}

Relation Relation::operator||(Relation& other) {
    return Relation([this, other](std::vector<z3::expr> vars){return this->f(vars) || other.f(vars);}, vars);
};

Relation Relation::operator&&(Relation& other) {
    return Relation([this, other](std::vector<z3::expr> vars){return this->f(vars) && other.f(vars);}, vars);
};

Relation Relation::operator!() {
    return Relation([this](std::vector<z3::expr> vars){return !(this->f(vars));}, vars);
};

std::ostream& operator<< (std::ostream &os, Relation R) {
    return os << R.get_relation();
}