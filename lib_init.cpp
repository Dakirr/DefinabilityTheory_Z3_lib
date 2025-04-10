#include "./lib_init.hpp"
#include <iostream>
#include <vector>

z3::context c;


// Relations from https://publications.hse.ru/pubs/share/direct/819810458.pdf
std::function<z3::expr(std::vector<z3::expr>)> get_function_A(int i, int n) {
    switch (i) {
        case 0:
            return [n](std::vector<z3::expr> V) {return (V[0] - V[1] == n);};
        case 1:
            return [n](std::vector<z3::expr> V) {return (((V[0] - V[1] == n) && (V[2] - V[3] == n)) || ((V[0] - V[1] == -n) && (V[2] - V[3] == -n)));};
        case 2:
            return [n](std::vector<z3::expr> V) {return (V[0] - V[1] == n) || (V[0] - V[1] == -n);};
        default:
            raise_error("Value", "get_function_A: expected 0 <= i <= 2, got i == " + std::to_string(i));
    }
}

std::function<z3::expr(z3::expr)> get_function_A_automorphism(int i, int n) {
    switch (i) {
        case 0:
            return [n](z3::expr x) {return (z3::ite(x%n == 0, x+n, x));};
        case 1:
            return [n](z3::expr x) {return (-x);};
        case 2:
            return [n](z3::expr x) {return (z3::ite(x%n == 0, -x, x));};
        default:
            raise_error("Value", "get_function_A_Automorphism: expected 0 <= i <= 2, got i == " + std::to_string(i));
    }
}

std::pair<std::function<z3::expr(z3::expr)>, z3::expr_vector> get_function_A_automorphism_group(int i, int n) {
    switch (i) {
        case 0:
            {
                z3::expr_vector params(c);
                for (int j = 0; j < n; j++) {
                    params.push_back(c.int_const(("param_" + std::to_string(j)).c_str()));
                }
                
                return std::make_pair(
                    [n, params](z3::expr x) {
                    z3::expr ret = x;
                    for (int j = 0; j < n; j++) {
                        ret = z3::ite(x%n == j, x+n*params[j], ret);
                    };
                    return ret;
                    },
                    params
                );
                
            }       
        case 1:
        {
            z3::expr_vector params(c);
            for (int j = 0; j < n; j++) {
                params.push_back(c.int_const(("param_" + std::to_string(j)).c_str()));
            }
            z3::expr param_bool = c.bool_const(std::string("param_is_negative").c_str());
            auto f = [n, params, param_bool](z3::expr x) {
                z3::expr ret = x;
                for (int j = 0; j < n; j++) {
                    ret = z3::ite(x%n == j, z3::ite(param_bool, -x+n*params[j], x+n*params[j]), ret);
                };
                return ret;};
            params.push_back(param_bool);
            return std::make_pair(f, params);
        };
        case 2:
        {
            z3::expr_vector params(c);
            z3::expr_vector bool_params(c);
            for (int j = 0; j < n; j++) {
                params.push_back(c.int_const(("param_" + std::to_string(j)).c_str()));
                bool_params.push_back(c.bool_const(("param_is_negative_" + std::to_string(j)).c_str()));
            }
            auto f = [n, params, bool_params](z3::expr x) {
                z3::expr ret = x;
                for (int j = 0; j < n; j++) {
                    ret = z3::ite(x%n == j, z3::ite(bool_params[j], -x+n*params[j], x+n*params[j]), ret);
                };
                return ret;
            };

            for (int j = 0; j < n; j++) {
                params.push_back(bool_params[j]);
            }
            return std::make_pair(f, params);
        }
        default:
            raise_error("Value", "get_function_A_Automorphism: expected 0 <= i <= 2, got i == " + std::to_string(i));
    }
}







