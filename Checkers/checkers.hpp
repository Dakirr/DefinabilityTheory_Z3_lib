#ifndef CHECKERS_HPP
#define CHECKERS_HPP
#include "../Generators/generate.hpp"
#include "../Relation/Relation.hpp"
#include "../Automorphism/Automorphism.hpp"
#include "../GridRelation/GridRelation.hpp"
#include "../GridAutomorphism/GridAutomorphism.hpp"


using QuFunction = std::pair<std::function<z3::expr(z3::expr)>, z3::expr_vector>;
using Qu2Function = std::pair<std::function<IntVec(IntVec)>, z3::expr_vector>;

z3::expr get_checker_relation_defines(Relation R, Relation S);
z3::expr get_checker_automorphism_saved(Relation R, Automorphism A);
int check_is_between (Relation R, std::pair<Relation, QuFunction> A1_p, std::pair<Relation, QuFunction> A2_p);

z3::expr get_checker_relation_defines(GridRelation R, GridRelation S);
z3::expr get_checker_automorphism_saved(GridRelation R, GridAutomorphism A);
int check_is_between (GridRelation R, std::pair<GridRelation, Qu2Function> A1_p, std::pair<GridRelation, Qu2Function> A2_p);

#endif