/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

extern "C" void py_pow2_above(int*, int*);
extern "C" void f_sphere_vol2rad(const double*, double*);

auto pow2_above(int n) {
    int res;
    py_pow2_above(&n, &res);
    return res;
}

double sphere_vol2rad(double v) {
    double rad;
    f_sphere_vol2rad(&v, &rad);
    return rad;
}

extern "C" double py_deg2rad(double);

