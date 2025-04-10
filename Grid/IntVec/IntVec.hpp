#ifndef INTVEC_HPP
#define INTVEC_HPP

#include "../../lib_init.hpp"
#include "../../Error/Error.hpp"

struct IntVec {
    public:
    z3::expr first;
    z3::expr second;
    IntVec(z3::expr O, z3::expr T): first(O), second(T) {};
    IntVec operator+(IntVec other);
    IntVec operator-();
    IntVec operator-(IntVec other);
    IntVec operator*(z3::expr other);
    IntVec operator/(z3::expr other);
    z3::expr operator[](int i);
};
IntVec operator*(z3::expr first, IntVec second);



#endif