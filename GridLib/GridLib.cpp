#include "GridLib.hpp"
// Отношения из основной статьи

GridRelation get_GridRelation_AD(IntVec q1, IntVec q2, int n, std::vector<IntVec>& vars) {
    return GridRelation(GridFunction<std::vector<IntVec>>(
        [n](std::vector<IntVec> V, Grid G){
            z3::expr ret = c.bool_val(false);
            for (int i = 0; i < n+1; i++) {
                ret = ret || ((V[0][0] + i*G.x[0] + (n-i)*G.y[0] == V[1][0]) && (V[0][0] + i*G.x[1] + (n-i)*G.y[1] == V[1][0])); 
            }    
            return ret;
        }
    ), vars, Grid(q1, q2));
}

GridRelation get_GridRelation_D(IntVec q1, IntVec q2, IntVec k, std::vector<IntVec>& vars) {
    auto k0 = k[0];
    auto k1 = k[1];
    return GridRelation(GridFunction<std::vector<IntVec>>(
        [k0, k1](std::vector<IntVec> V, Grid G){
            return ((V[0][0] + k0*G.x[0] + k1*G.y[0] == V[1][0]) && (V[0][1] + k0*G.x[1] + k1*G.y[1] == V[1][0])) ||
                   ((V[1][0] + k0*G.x[0] + k1*G.y[0] == V[0][0]) && (V[1][1] + k0*G.x[1] + k1*G.y[1] == V[0][0])); 
        }
    ), vars, Grid(q1, q2));
}

GridRelation get_GridRelation_D2(IntVec q1, IntVec q2, int n, std::vector<IntVec>& vars) {
    return (get_GridRelation_AD( q1,  q2, n, vars)) ||
           (get_GridRelation_AD(-q1,  q2, n, vars)) ||
           (get_GridRelation_AD( q1, -q2, n, vars)) ||
           (get_GridRelation_AD(-q1, -q2, n, vars)); 
}

std::pair<std::function<IntVec(IntVec)>, z3::expr_vector> get_function_AD_automorphism_group(IntVec q1, IntVec q2, int n) {
    z3::expr_vector r(c);
    z3::expr pb = c.bool_val("param_bool");
    for (int i = 0; i < n; i++) {
        r.push_back(c.int_val(("param_x_" + std::to_string(i)).c_str()));
    }
    for (int i = 0; i < n; i++) {
        r.push_back(c.int_val(("param_y_" + std::to_string(i)).c_str()));
    }
    r.push_back(pb);
    z3::expr q1_0 = q1[0];
    z3::expr q1_1 = q1[1];
    z3::expr q2_0 = q2[0];
    z3::expr q2_1 = q2[1];
    
    return std::make_pair(
        [q1_0, q1_1, q2_0, q2_1, n, r, pb](IntVec X){
            z3::expr xp = c.int_val(0);
            z3::expr yp = c.int_val(0);
            for (int i = 0; i < n; i++) {
                xp = z3::ite((X[0] + X[1])%n == i, X[0] + q1_0*r[i] + q2_0*r[n+i], xp);
                yp = z3::ite((X[0] + X[1])%n == i, X[1] + q1_1*r[i] + q2_1*r[n+i], yp);
            }
            return  IntVec(
                        z3::ite(pb, xp, yp),
                        z3::ite(pb, yp, xp)
                    );
        }, r
    );
}

std::pair<std::function<IntVec(IntVec)>, z3::expr_vector> get_function_D2_automorphism_group(IntVec q1, IntVec q2, int n) {
    z3::expr_vector r(c);
    z3::expr pb1 = c.bool_val("param_bool1");
    z3::expr pb2 = c.bool_val("param_bool2");
    z3::expr pb3 = c.bool_val("param_bool2");
    
    for (int i = 0; i < n; i++) {
        r.push_back(c.int_val(("param_x_" + std::to_string(i)).c_str()));
    }
    for (int i = 0; i < n; i++) {
        r.push_back(c.int_val(("param_y_" + std::to_string(i)).c_str()));
    }
    r.push_back(pb1);
    r.push_back(pb2);
    r.push_back(pb3);
    z3::expr q1_0 = q1[0];
    z3::expr q1_1 = q1[1];
    z3::expr q2_0 = q2[0];
    z3::expr q2_1 = q2[1];
    
    return std::make_pair(
        [q1_0, q1_1, q2_0, q2_1, n, r, pb1, pb2, pb3](IntVec X){
            z3::expr xp = c.int_val(0);
            z3::expr yp = c.int_val(0);
            for (int i = 0; i < n; i++) {
                xp = z3::ite((X[0] + X[1])%n == i, X[0] + q1_0*r[i] + q2_0*r[n+i], xp);
                yp = z3::ite((X[0] + X[1])%n == i, X[1] + q1_1*r[i] + q2_1*r[n+i], yp);
            }
            return  IntVec(
                        z3::ite(pb1, xp, z3::ite(pb2,yp,z3::ite(pb3,-xp,-yp))),
                        z3::ite(pb1, yp, z3::ite(pb2,-xp,z3::ite(pb3,-yp,xp)))
                    );
        }, r
    );
}
