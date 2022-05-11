/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#include <cstdio>

char _strerror(const int ncerr) {
  //printf("C++ strerror");
  return 0;
}

extern "C" char nf90_strerror(const int *ncerr) {
  return _strerror(*ncerr);
}
