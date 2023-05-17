/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "util.hpp"

int pow2_above(int n) {
    int res;
    py_pow2_above(&n, &res);
    return res;
}

double sphere_vol2rad(double v) {
    double rad;
    f_sphere_vol2rad(&v, &rad);
    return rad;
}

double rad2diam(double rad) {
    double d;
    f_rad2diam(&rad, &d);
    return  d;
}

double sphere_rad2vol(double rad) {
    double v;
    f_sphere_rad2vol(&rad, &v);
    return v;
}

double diam2rad(double d) {
    double rad;
    f_diam2rad(&d, &rad);
    return rad;
}
