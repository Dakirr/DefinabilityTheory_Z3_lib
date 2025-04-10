#include "Automorphism.hpp"

Relation Automorphism::apply(Relation R) {
    std::vector<z3::expr> v;
    for (auto i = R.vars.begin(); i != R.vars.end(); ++i) {
        v.push_back(f(*i));
    }

    return Relation(R.f, v);
};

std::ostream& operator<< (std::ostream &os, Automorphism R) {
    z3::expr x = c.int_const("x");

    return os << "x -> " << R.f(x);//<< R.f(c.int_val("x"));
}; 