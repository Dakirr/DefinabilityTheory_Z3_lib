#ifndef GRID_LIB_HPP
#define GRID_LIB_HPP

#include "../GridAutomorphism/GridAutomorphism.hpp"

GridRelation get_GridRelation_AD(IntVec q1, IntVec q2, int n, std::vector<IntVec>& vars);
GridRelation get_GridRelation_D(IntVec q1, IntVec q2, IntVec k, std::vector<IntVec>& vars);
GridRelation get_GridRelation_D2(IntVec q1, IntVec q2, int n, std::vector<IntVec>& vars);
std::pair<std::function<IntVec(IntVec)>, z3::expr_vector> get_function_AD_automorphism_group(IntVec q1, IntVec q2, int n);
std::pair<std::function<IntVec(IntVec)>, z3::expr_vector> get_function_D2_automorphism_group(IntVec q1, IntVec q2, int n);

#endif