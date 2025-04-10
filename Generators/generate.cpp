#include "generate.hpp"
#include <iostream>
#include <vector>
#include <functional>
#include <map>

bool is_in_vector(std::vector<z3::expr>& expressions, z3::expr e) {
    for (auto it = expressions.begin(); it != expressions.end(); it++) {
        z3::solver s(c);
        s.add(!(e == *it));
        
        if (s.check() == z3::unsat) {
            return true;
            break; 
        }
    }
    return false; 
}

std::vector<z3::expr> generate_1dim(int x_n, int max_k, std::vector<z3::expr>& vars) {
    std::vector<z3::expr> variables = std::vector<z3::expr>();
    
    for (int i = 0; i < x_n; i++) {
        variables.push_back(c.int_const(("x" + std::to_string(i)).c_str()));
    }

    std::vector<z3::expr> parts = std::vector<z3::expr>();
    for (auto it1 = variables.begin(); it1 != variables.end(); it1++) {
        std::cout << *it1 << std::endl;
        for (auto it2 = variables.begin(); it2 != variables.end(); it2++) {
            if (it1 != it2) {
                for (int i = 1; i < max_k + 1; i++) {
                    parts.push_back(*it1 + i == *it2);
                    //std::cout << "[" << *it1 << " + " << i << " == " << *it2 << "]: " << (*it1 + i == *it2) << std::endl;
                }
            }
        }
    }

    std::vector<z3::expr> disjuncts = std::vector<z3::expr>();
    disjuncts.push_back(c.bool_val(true));
    disjuncts.push_back(c.bool_val(false));
    for (auto it1 = parts.begin(); it1 != parts.end(); it1++) {
        std::cout << *it1 << std::endl;
        std::vector<z3::expr> tmp = std::vector<z3::expr>();
        for (auto it2 = disjuncts.begin(); it2 != disjuncts.end(); it2++) {
            if (!is_in_vector(tmp, *it1&&*it2)) tmp.push_back(*it1&&*it2);
            if (!is_in_vector(tmp, (!*it1)&&*it2)) tmp.push_back((!*it1)&&*it2);
            //std::cout << "[" << (*it1|*it2) << ", " << ((!*it1)|*it2) << "]" << std::endl;
        }
        disjuncts = tmp;
    }
    

    std::vector<z3::expr> expressions = std::vector<z3::expr>();
    expressions.push_back(c.bool_val(false));
    for (auto it1 = disjuncts.begin(); it1 != disjuncts.end(); it1++) {
        std::vector<z3::expr> tmp = std::vector<z3::expr>();
        for (auto it2 = expressions.begin(); it2 != expressions.end(); it2++) {
            //std::cout << "[" << *it1 << ", " << *it2 << "]: " << (*it1||*it2) << std::endl;
            if (!is_in_vector(expressions, *it1||*it2) && !is_in_vector(tmp, (*it1||*it2))) tmp.push_back(*it1||*it2);
        }
        for (auto it2 = tmp.begin(); it2 != tmp.end(); it2++) {
            expressions.push_back(*it2);
        }
    }
    if (!is_in_vector(expressions, c.bool_val(true))) expressions.push_back(c.bool_val(true));
    return expressions;
}

void get_variables(z3::expr const & e, std::vector<z3::expr>& expressions) {
    if (e.is_app()) {
        int N = e.num_args();
        for (int i = 0; i < N; i++) {
            get_variables(e.arg(i), expressions);
        }
        if (!(e.is_numeral()) && e.is_const()) expressions.push_back(e);
    }
    else if (e.is_quantifier()) {
        get_variables(e.body(), expressions);
    }
};

template <typename T>
std::vector<std::vector<T>> get_combinations(std::vector<T>& free_vars, std::vector<T>& quantifier_vars, int max_quantifier_amount) {
    int N = free_vars.size();
    std::map<int, std::vector<std::pair<int, std::vector<T>>>> all_combinations;

    for (int i = 0; i < N+1; i++) {
        all_combinations[i] = std::vector<std::pair<int, std::vector<T>>>();
        if (i == 0) {
            all_combinations[i].push_back(std::make_pair(0, std::vector<T>()));
        } else {
            for (auto it = all_combinations[i-1].begin(); it != all_combinations[i-1].end(); it++) {
                for (auto it2 = free_vars.begin(); it2 != free_vars.end(); it2) {
                    all_combinations[i].push_back(*it);
                    all_combinations[i].back().second.push_back(*it2);
                }
                for (auto it2 = quantifier_vars.begin(); it2 != quantifier_vars.end(); it2) {
                    if (it->first + 1 <= max_quantifier_amount) {
                        all_combinations[i].push_back(*it);
                        all_combinations[i].back().first += 1;
                        all_combinations[i].back().second.push_back(*it2);
                    }
                }
            }
        }
    }
    std::vector<std::vector<T>> ret;

    for (auto it = all_combinations[N].begin(); it != all_combinations[N].end(); it++) {
        ret.push_back(it->second);
    }

    return ret;
}

std::vector<z3::expr> generate_from(Relation R, int quantifier_amount) {
    std::vector<z3::expr> free_vars;
    std::vector<z3::expr> quantifier_vars;
    

    get_variables(R.get_relation(), free_vars);
    
    int R_size = free_vars.size();

    std::vector<z3::expr> vars = std::vector<z3::expr>(free_vars);
    for (int i = 0; i < quantifier_amount; i++) {
        vars.push_back(c.int_const(("quantifier_" + std::to_string(i)).c_str()));
        quantifier_vars.push_back(c.int_const(("quantifier_" + std::to_string(i)).c_str()));
    }
    
    Relation R1 = Relation(R, vars);
    auto combinations = get_combinations(free_vars, quantifier_vars, quantifier_amount);
    
    std::vector<z3::expr> parts;
    for (int i = 0; i < combinations.size(); i++) {
        parts.push_back(R1.get_relation(combinations[i]));
    }

    std::vector<z3::expr> disjuncts = std::vector<z3::expr>();;
    disjuncts.push_back(c.bool_val(true));
    disjuncts.push_back(c.bool_val(false));
    for (auto it1 = parts.begin(); it1 != parts.end(); it1++) {
        std::vector<z3::expr> tmp = std::vector<z3::expr>();
        for (auto it2 = disjuncts.begin(); it2 != disjuncts.end(); it2++) {
            if (!is_in_vector(tmp, *it1&&*it2)) tmp.push_back(*it1&&*it2);
            if (!is_in_vector(tmp, (!*it1)&&*it2)) tmp.push_back((!*it1)&&*it2);
        }
        disjuncts = tmp;
    }

    std::vector<z3::expr> expressions = std::vector<z3::expr>();
    expressions.push_back(c.bool_val(false));
    for (auto it1 = disjuncts.begin(); it1 != disjuncts.end(); it1++) {
        std::vector<z3::expr> tmp = std::vector<z3::expr>();
        for (auto it2 = expressions.begin(); it2 != expressions.end(); it2++) {
            if (!is_in_vector(expressions, *it1||*it2) && !is_in_vector(tmp, (*it1||*it2))) tmp.push_back(*it1||*it2);
        }
        for (auto it2 = tmp.begin(); it2 != tmp.end(); it2++) {
            expressions.push_back(*it2);
        }
    }
    if (!is_in_vector(expressions, c.bool_val(true))) expressions.push_back(c.bool_val(true));

    std::vector<z3::expr> ret;
    z3::expr_vector qV(c);
    for (int i = 0; i < quantifier_vars.size(); i++) {
        qV.push_back(quantifier_vars[i]);
    }
    for (int i = 0; i < expressions.size(); i++) {
        ret.push_back(z3::exists(qV, expressions[i])); 
    }

    return ret;
}

std::vector<z3::expr> generate_from(GridRelation R, int quantifier_amount) {
    std::vector<IntVec> free_vars;
    std::vector<IntVec> quantifier_vars;

    free_vars = R.vars;
    
    int R_size = free_vars.size();

    std::vector<IntVec> vars = free_vars;
    for (int i = 0; i < quantifier_amount; i++) {
        vars.push_back(IntVec(c.int_const(("quantifier_nom_" + std::to_string(i)).c_str()),
                              c.int_const(("quantifier_denom_" + std::to_string(i)).c_str())));
        quantifier_vars.push_back(IntVec(c.int_const(("quantifier_nom_" + std::to_string(i)).c_str()),
                                         c.int_const(("quantifier_denom_" + std::to_string(i)).c_str())));;
    }
    
    GridRelation R1 = GridRelation(R, vars);
    auto combinations = get_combinations(free_vars, quantifier_vars, quantifier_amount);
    
    std::vector<z3::expr> parts;
    for (int i = 0; i < combinations.size(); i++) {
        parts.push_back(R1.get_relation(combinations[i]));
    }

    std::vector<z3::expr> disjuncts = std::vector<z3::expr>();
    disjuncts.push_back(c.bool_val(true));
    disjuncts.push_back(c.bool_val(false));
    for (auto it1 = parts.begin(); it1 != parts.end(); it1++) {
        std::vector<z3::expr> tmp = std::vector<z3::expr>();
        for (auto it2 = disjuncts.begin(); it2 != disjuncts.end(); it2++) {
            if (!is_in_vector(tmp, *it1&&*it2)) tmp.push_back(*it1&&*it2);
            if (!is_in_vector(tmp, (!*it1)&&*it2)) tmp.push_back((!*it1)&&*it2);
        }
        disjuncts = tmp;
    }

    std::vector<z3::expr> expressions = std::vector<z3::expr>();
    expressions.push_back(c.bool_val(false));
    for (auto it1 = disjuncts.begin(); it1 != disjuncts.end(); it1++) {
        std::vector<z3::expr> tmp = std::vector<z3::expr>();
        for (auto it2 = expressions.begin(); it2 != expressions.end(); it2++) {
            if (!is_in_vector(expressions, *it1||*it2) && !is_in_vector(tmp, (*it1||*it2))) tmp.push_back(*it1||*it2);
        }
        for (auto it2 = tmp.begin(); it2 != tmp.end(); it2++) {
            expressions.push_back(*it2);
        }
    }
    if (!is_in_vector(expressions, c.bool_val(true))) expressions.push_back(c.bool_val(true));

    std::vector<z3::expr> ret;
    z3::expr_vector qV(c);
    for (int i = 0; i < quantifier_vars.size(); i++) {
        qV.push_back(quantifier_vars[i][0]);
        qV.push_back(quantifier_vars[i][1]);
    }
    for (int i = 0; i < expressions.size(); i++) {
        ret.push_back(z3::exists(qV, expressions[i])); 
    }

    return ret;
}