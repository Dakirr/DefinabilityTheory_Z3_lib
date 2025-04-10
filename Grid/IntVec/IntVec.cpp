#include "IntVec.hpp"

IntVec IntVec::operator+(IntVec other) {
        return IntVec(first + other.first, second + other.second); 
}

IntVec IntVec::operator-() {
    return IntVec(-first, -second); 
}

IntVec IntVec::operator-(IntVec other) {
    return this->operator+(-other); 
}

IntVec IntVec::operator*(z3::expr other) {
    return IntVec(other*first, other*second);
}

IntVec IntVec::operator/(z3::expr other) {
    return IntVec(first/other, second/other);
}

IntVec operator*(z3::expr first, IntVec second) {
    return second * first;
}

z3::expr IntVec::operator[](int i) {
    if (i == 0) {
        return first;
    } else if (i == 1) {
        return second;
    } else {  
        raise_error("Index", "IntVec[]: 0 <= i <= 2 expected, got " + std::to_string(i));
    }
};