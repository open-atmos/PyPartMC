/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include <stdexcept>
#include <sstream>

void _mpi_abort(const int errorcode) {
  std::ostringstream oss;
  oss << errorcode << std::endl;
  auto err = std::runtime_error(oss.str());
  throw err;
}

extern "C" void mpi_abort(const int *comm, const int *errorcode, const int *ierror) {
  _mpi_abort(*errorcode);
}

