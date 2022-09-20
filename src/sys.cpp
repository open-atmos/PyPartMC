/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include <stdexcept>
#include <sstream>

void cpp_stop(int code) {
  std::ostringstream str;
  str << code;
  throw std::runtime_error(str.str());
}

extern "C" void c_stop(int code) {
  cpp_stop(code);
}
