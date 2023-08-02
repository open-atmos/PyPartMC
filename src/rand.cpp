/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "rand.hpp"

void rand_init(int seed) {
  f_pmc_srand(&seed);
}

double rand_normal(double mean, double stddev) {
  double val;

  f_rand_normal(&mean, &stddev, &val);

  return val;
}
