/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include <iostream>
#include <cassert>

#include "gimmicks.hpp"


template <typename T>
void put_var(
  const int &ncid,
  const int &varid,
  const T *values,
  const int &start,
  const int &count
) {
  std::cerr << "C++ put_var<T> ..." << std::endl;
  //get_fake_netcdf_json() = std::make_unique<nlohmann::json>(nlohmann::json{{"aqq", 44}}); // TODO!
}

extern "C" 
void nf90_put_var_dbl(
  const int *ncid,
  const int *varid,
  const double *values,
  const int *start,
  const int *count
) {
  put_var(*ncid, *varid, values, *start, *count);
}

extern "C"
void nf90_put_var_int(
  const int *ncid,
  const int *varid,
  const int *values, // TODO: fortran type
  const int *start,
  const int *count
) {
  put_var(*ncid, *varid, values, *start, *count);
}

template <typename T>
void get_var(
  const int &ncid,
  const int &varid,
  const T *values,
  const int &start,
  const int &count
) {
  std::cerr << "C++ get_var<T> ..." << std::endl;
}

extern "C" void nf90_get_var_dbl(
  const int *ncid,
  const int *varid,
  const double *values,
  const int *start,
  const int *count
) {
    get_var(*ncid, *varid, values, *start, *count);
}

extern "C" 
void nf90_get_var_int(const int *ncid, const int *varid, const int *values, // TODO: fortran type
  const int *start,
  const int *count
) {
    get_var(*ncid, *varid, values, *start, *count);
}


void inq_varid(const int &ncid, const std::string &name, int *varid) {
    *varid = gimmick_ptr()->varid(name);
}

extern "C" 
void nf90_inq_varid_str(const int *ncid, const char *name_data, int *name_size, int *varid) {
    inq_varid(*ncid, std::string(name_data, *name_size), varid); // TODO: change into string_view?
}

