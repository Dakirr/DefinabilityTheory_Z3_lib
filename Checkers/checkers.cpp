#include "checkers.hpp"
//////
int stage2_quantifier_amount = 3;
int timeout_time = 100000;
//////

z3::expr get_checker_relation_defines(Relation R, Relation S) {
    z3::sort I = c.int_sort();
    z3::func_decl f = function("f", I, I);
    z3::expr_vector vars(c);
    for (int i = 0; i < R.vars.size(); i++) {
        const z3::expr tmp = R.vars[i];
        vars.push_back(tmp);
    }        
    return ((z3::forall(vars, R.get_relation() == R.get_relation(f))) && !(z3::forall(vars, S.get_relation() == S.get_relation(f))));
}

z3::expr get_checker_automorphism_saved(Relation R, Automorphism A) {
    return !(R.get_relation() == A.apply(R).get_relation());
} 

int check_something_in_between(std::pair<int,int> A1_ind, std::pair<int,int> A2_ind) {
    z3::set_param("timeout", timeout_time);

    auto M1 = get_function_A_automorphism_group(A1_ind.first, A1_ind.second);
    auto M2 = get_function_A_automorphism_group(A2_ind.first, A2_ind.second);
    z3::expr x = c.int_const("x");
    z3::solver s(c);
    z3::func_decl f = z3::function("f", c.int_sort(), c.int_sort());
    s.add(z3::forall(x, z3::exists(M2.second, M2.first(x) == f(x)) && !z3::exists(M1.second, M1.first(x) == f(x))));
    if (s.check() == z3::sat) {
        return 1;
    } else if (s.check() == z3::unsat) {
        return 0;
    } else {
        return -1;
    }
}


int check_is_between (Relation R, std::pair<Relation, QuFunction> A1_p, std::pair<Relation, QuFunction> A2_p) {
    z3::set_param("timeout", timeout_time);
    z3::solver s(c);

    // Try 1: Automorphism group search.
    // Tries to find an automorphism that saves R, belongs to the group of A2 automorphisms and not belong to the group of A1. 
    // If there is one, R lays between A1 and A2.

    std::cout << "Try 1. Automorphism group search: ";

    auto A1 = A1_p.first;
    auto A2 = A2_p.first;
    auto M1 = A1_p.second;
    auto M2 = A2_p.second;
    z3::expr x = c.int_const("x");
    z3::func_decl f = z3::function("f", c.int_sort(), c.int_sort());
    s.add(z3::forall(x, z3::exists(M2.second, M2.first(x) == f(x)) && !z3::exists(M1.second, M1.first(x) == f(x))));
    s.add(R.get_relation(f) == R.get_relation());
    if (s.check() == z3::sat) {
        return 1;
    } else if (s.check() == z3::unsat) {
        return 0;
    }
    std::cout << "unknown\n";

    // Try 2: Quantifiers checker.
    // Tries to find any f such as forall{x} R(x) = R(f(x)) and !(A1(f(x)) = A1(x))
    // If there is one, tries to find g such as forall{x} A2(x) = A2(g(x)) and !(R(g(x)) = R(x))
    // If there is one, R lays between A1 and A2.

    std::cout << "Try 2. Quantifiers checker: ";

    z3::solver s3(c);
    s3.add(get_checker_relation_defines(A1, R));
    s3.add(!get_checker_relation_defines(R, A1));
    if (s3.check() == z3::unsat) {
        z3::solver s4(c);
        s4.add(get_checker_relation_defines(R, A2));
        s4.add(!get_checker_relation_defines(A2, R));
        if (s4.check() == z3::unsat) {
            return 1;
        } else if (s4.check() == z3::sat) {
            return 0;
        }
    } else if (s3.check() == z3::sat) {
        return 0;
    }
    std::cout << "unknown\n";


    // Try 3: Limited search.
    // Tries to build every FDNF form from A1 with no more than stage2_quantifier_amount quantifier variables
    // and test, whether the built formula is equal to R (being the pair (A1, R)).
    // The same process is repeated for pairs (R, A1) and (R, A2).

    std::cout << "Try 3. Limited search: ";
    auto generated = generate_from(A1, stage2_quantifier_amount);
    auto generated2 = generate_from(R, stage2_quantifier_amount);
    
    
    for (auto it = generated.begin(); it != generated.end(); it++) {
        z3::solver s2(c);
        s2.add(!(R.get_relation() == *it));
        if (s2.check() == z3::unsat) {
            for (auto it2 = generated2.begin(); it2 != generated2.end(); it2++) {
                z3::solver s3(c);
                s3.add(!(A2.get_relation() == *it2));
                if (s3.check() == z3::unsat) {
                    for (auto it3 = generated.begin(); it3 != generated.end(); it3) {
                        z3::solver s4(c);
                        s4.add(!(A1.get_relation() == *it3));
                        if (s4.check() == z3::unsat) {
                            return 1;
                        }
                    }
                }
            } 
        } 
    }
    
    return -1;
}


z3::expr get_checker_relation_defines(GridRelation R, GridRelation S) {
    z3::sort I = c.int_sort();
    z3::func_decl f1 = function("f1", I, I);
    z3::func_decl f2 = function("f2", I, I);
    z3::expr_vector vars(c);
    for (int i = 0; i < R.vars.size(); i++) {
        const z3::expr tmp1 = R.vars[i][0];
        const z3::expr tmp2 = R.vars[i][1];
        vars.push_back(tmp1);
        vars.push_back(tmp2);
    }        
    return ((z3::forall(vars, R.get_relation() == R.get_relation(f1, f2))) && !(z3::forall(vars, S.get_relation() == S.get_relation(f1, f2))));

};

z3::expr get_checker_automorphism_saved(GridRelation R, GridAutomorphism A) {
    return !(R.get_relation() == A.apply(R).get_relation());
};


int check_is_between (GridRelation R, std::pair<GridRelation, Qu2Function> A1_p, std::pair<GridRelation, Qu2Function> A2_p) {
    z3::set_param("timeout", timeout_time);
    z3::solver s(c);

    // Try 1: Automorphism group search.
    // Tries to find an automorphism that saves R, belongs to the group of A2 automorphisms and not belong to the group of A1. 
    // If there is one, R lays between A1 and A2.

    std::cout << "Try 1. Automorphism group search: ";

    auto A1 = A1_p.first;
    auto A2 = A2_p.first;
    auto M1 = A1_p.second;
    auto M2 = A2_p.second;
    z3::expr x1 = c.int_const("x1");
    z3::expr x2 = c.int_const("x2");
    z3::func_decl f1 = z3::function("f", c.int_sort(), c.int_sort());
    z3::func_decl f2 = z3::function("f", c.int_sort(), c.int_sort());
    s.add(z3::forall(x1, x2, z3::exists(M2.second, M2.first(IntVec(x1, x2))[0] == f1(x1) && M2.first(IntVec(x1, x2))[1] == f2(x2))) && !z3::exists(M1.second, M1.first(IntVec(x1, x2))[0] == f1(x1) && M1.first(IntVec(x1, x2))[1] == f2(x2)));
    s.add(R.get_relation(f1, f2) == R.get_relation());
    if (s.check() == z3::sat) {
        return 1;
    } else if (s.check() == z3::unsat) {
        return 0;
    }
    std::cout << "unknown\n";

    // Try 2: Quantifiers checker.
    // Tries to find any f such as forall{x} R(x) = R(f(x)) and !(A1(f(x)) = A1(x))
    // If there is one, tries to find g such as forall{x} A2(x) = A2(g(x)) and !(R(g(x)) = R(x))
    // If there is one, R lays between A1 and A2.

    std::cout << "Try 2. Quantifiers checker: ";

    z3::solver s3(c);
    s3.add(get_checker_relation_defines(A1, R));
    s3.add(!get_checker_relation_defines(R, A1));
    if (s3.check() == z3::unsat) {
        z3::solver s4(c);
        s4.add(get_checker_relation_defines(R, A2));
        s4.add(!get_checker_relation_defines(A2, R));
        if (s4.check() == z3::unsat) {
            return 1;
        } else if (s4.check() == z3::sat) {
            return 0;
        }
    } else if (s3.check() == z3::sat) {
        return 0;
    }
    std::cout << "unknown\n";


    // Try 3: Limited search.
    // Tries to build every FDNF form from A1 with no more than stage2_quantifier_amount quantifier variables
    // and test, whether the built formula is equal to R (being the pair (A1, R)).
    // The same process is repeated for pairs (R, A1) and (R, A2).

    std::cout << "Try 3. Limited search: ";
    auto generated = generate_from(A1, stage2_quantifier_amount);
    auto generated2 = generate_from(R, stage2_quantifier_amount);
    
    
    for (auto it = generated.begin(); it != generated.end(); it++) {
        z3::solver s2(c);
        s2.add(!(R.get_relation() == *it));
        if (s2.check() == z3::unsat) {
            for (auto it2 = generated2.begin(); it2 != generated2.end(); it2++) {
                z3::solver s3(c);
                s3.add(!(A2.get_relation() == *it2));
                if (s3.check() == z3::unsat) {
                    for (auto it3 = generated.begin(); it3 != generated.end(); it3) {
                        z3::solver s4(c);
                        s4.add(!(A1.get_relation() == *it3));
                        if (s4.check() == z3::unsat) {
                            return 1;
                        }
                    }
                }
            } 
        } 
    }
    
    return -1;
};
