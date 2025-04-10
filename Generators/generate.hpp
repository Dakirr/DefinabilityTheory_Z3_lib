#ifndef GENERATE_HPP
#define GENERATE_HPP

#include "../lib_init.hpp"
#include "../Relation/Relation.hpp" 
#include "../GridRelation/GridRelation.hpp"
#include <set>

bool is_in_vector(std::vector<z3::expr>& expressions, z3::expr e);
std::vector<z3::expr> generate_1dim(int x_n, int max_k, std::vector<z3::expr>& vars);
std::vector<z3::expr> generate_from(Relation R, int quantifier_amount);
std::vector<z3::expr> generate_from(GridRelation R, int quantifier_amount);
#endif