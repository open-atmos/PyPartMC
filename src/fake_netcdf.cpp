/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#include <iostream>
#include <cassert>

#include "fake_netcdf.hpp"


template <class X>
X& singleton()
{
    static X x;
    return x;
}

std::unique_ptr<nlohmann::json> &get_fake_netcdf_json() {
    return singleton<std::unique_ptr<nlohmann::json>>();
}

template <typename T>
int put_var(
  const int &ncid,
  const int &varid,
  const T *values,
  const int &start,
  const int &count
) {
  std::cerr << "C++ put_var<T> ..." << std::endl;
  get_fake_netcdf_json() = std::make_unique<nlohmann::json>(nlohmann::json{{"aqq", 44}}); // TODO!
  return 0;  // TODO
}

extern "C" int nf90_put_var_dbl(
  const int *ncid,
  const int *varid,
  const double *values,
  const int *start,
  const int *count
) {
  return put_var(*ncid, *varid, values, *start, *count);
}

extern "C" int nf90_put_var_int(
  const int *ncid,
  const int *varid,
  const int *values, // TODO: fortran type
  const int *start,
  const int *count
) {
  return put_var(*ncid, *varid, values, *start, *count);
}

template <typename T>
int get_var(
  const int &ncid,
  const int &varid,
  const T *values,
  const int &start,
  const int &count
) {
  std::cerr << "C++ get_var<T> ..." << std::endl;
  return 0;  // TODO
}

extern "C" int nf90_get_var_dbl(
  const int *ncid,
  const int *varid,
  const double *values,
  const int *start,
  const int *count
) {
  return get_var(*ncid, *varid, values, *start, *count);
}

extern "C" int nf90_get_var_int(
  const int *ncid,
  const int *varid,
  const int *values, // TODO: fortran type
  const int *start,
  const int *count
) {
  return get_var(*ncid, *varid, values, *start, *count);
}
