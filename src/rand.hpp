/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

extern "C" void f_pmc_srand(const int*);
extern "C" void f_rand_normal(const double*, const double*, double*);
void rand_init(int seed);
double rand_normal(double mean, double stddev);
