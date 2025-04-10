#ifndef RELATION_HPP
#define RELATION_HPP
#include <vector>
#include "../lib_init.hpp"
#include <iostream>
#include <functional>

class Relation {
    public:
        std::function<z3::expr(std::vector<z3::expr>)> f;
        std::vector<z3::expr> vars;
        int dim;

        Relation(std::function<z3::expr(std::vector<z3::expr>)> f, std::vector<z3::expr> vars) : f(f), vars(vars) {};
        Relation(Relation other, std::vector<z3::expr> v): f(other.f), vars(v) {};
        Relation(Relation other, std::vector<int> indexes);

        
        z3::expr get_relation();
        z3::expr get_relation(std::vector<z3::expr> vars);
        z3::expr get_relation(z3::func_decl f);
        z3::expr get_relation(std::vector<int> indexes);
        Relation operator||(Relation& other);
        Relation operator&&(Relation& other);
        Relation operator!();
};

std::ostream& operator<< (std::ostream &os, Relation R);

#endif