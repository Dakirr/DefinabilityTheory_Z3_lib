#ifndef AUTOMORPHISM_HPP
#define AUTOMORPHISM_HPP
#include "../lib_init.hpp"
#include <functional>
#include "../Relation/Relation.hpp"

class Automorphism {
    public:
        std::function<z3::expr(z3::expr)> f;
        Automorphism(z3::func_decl f1): f(f1) {};
        Automorphism(std::function<z3::expr(z3::expr)> f) : f(f) {};
        Relation apply(Relation R);
};
std::ostream& operator<< (std::ostream &os, Automorphism R);

#endif