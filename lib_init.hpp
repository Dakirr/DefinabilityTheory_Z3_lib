#ifndef LIB_HPP
#define LIB_HPP
#include <z3++.h>
#include "./Error/Error.hpp"


extern z3::context c;
std::function<z3::expr(std::vector<z3::expr>)> get_function_A(int i, int n);
std::function<z3::expr(z3::expr)> get_function_A_automorphism(int i, int n);
std::pair<std::function<z3::expr(z3::expr)>, z3::expr_vector> get_function_A_automorphism_group(int i, int n);

#endif