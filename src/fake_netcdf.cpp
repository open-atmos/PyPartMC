/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#include <cstdio>
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
  const int *ncid,
  const int *varid,
  const T *values,
  const int *start,
  const int *count
) {
  get_fake_netcdf_json() = std::make_unique<nlohmann::json>(nlohmann::json{{"aqq", 44}}); // TODO!
  return 0;
}

extern "C" int nf90_put_var_dbl(
  const int *ncid,
  const int *varid,
  const double *values,
  const int *start,
  const int *count
) {
  return put_var(ncid, varid, values, start, count);
}

extern "C" int nf90_put_var_int(
  const int *ncid,
  const int *varid,
  const int *values,
  const int *start,
  const int *count
) {
  return put_var(ncid, varid, values, start, count);
}

