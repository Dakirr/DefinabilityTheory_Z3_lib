#include "lib_init.hpp"
#include "./Relation/Relation.hpp"
#include "./Automorphism/Automorphism.hpp"
#include "./Checkers/checkers.hpp"
#include "./Generators/generate.hpp"



#include <iostream>
#include <vector>
#include <functional>

//Quick example of non-definability
int main() {      
    std::vector<z3::expr> variables = std::vector<z3::expr>();
    int k = 10;
    for (int i = 0; i < 5; i++) {
        variables.push_back(c.int_const(("x" + std::to_string(i)).c_str()));
    }
    
    Relation R ([](std::vector<z3::expr> V){return ((V[0] - V[1] == 2) && (V[2] - V[3] == 3));}, variables);
    
    
    std::cout << "R between A(" << 1 << ", " << 3 << ") and A(" << 2 << ", " << 3 << "): \n";
    switch (check_is_between(R, {Relation(get_function_A(1, 3), R.vars), get_function_A_automorphism_group(1, 3)},\
                                {Relation(get_function_A(2, 3), R.vars), get_function_A_automorphism_group(2, 3)})) {
        case 1:
            std::cout << "true\n";
            break;
        case 0:
            std::cout << "false\n";
            break;
        case -1:
            std::cout << "unknown\n";
            break;
    }

    return 0;
}
