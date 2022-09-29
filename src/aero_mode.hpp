/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "pybind11/stl.h"

extern "C" void f_aero_mode_ctor(void *ptr) noexcept;
extern "C" void f_aero_mode_dtor(void *ptr) noexcept;
extern "C" void f_aero_mode_init(
    const void *ptr,
    const void *aero_data_ptr
) noexcept;
extern "C" void f_aero_mode_total_num_conc(
    const void *ptr,
    double *val
) noexcept;
extern "C" void f_aero_mode_num_conc(
       const void *ptr, const void *bin_grid_ptr,
       const void *aero_data_ptr_c, void *arr_data, const int *arr_size
) noexcept;
extern "C" void f_aero_mode_get_n_spec(
  const void *ptr,
  int *len
) noexcept;
extern "C" void f_aero_mode_get_vol_frac(
  const void *ptr,
  void *arr_data,
  const int *arr_size
) noexcept;
extern "C" void f_aero_mode_set_vol_frac(
  void *ptr,
  const void *arr_data,
  const int *arr_size
) noexcept;

struct AeroMode {
    PMCResource ptr;

    AeroMode(const AeroData &aero_data) :
        ptr(f_aero_mode_ctor, f_aero_mode_dtor)
    {
        f_aero_mode_init(ptr.f_arg(), aero_data.ptr.f_arg());
    }

    static double num_conc(const AeroMode &self){
       double val;
       f_aero_mode_total_num_conc(&self.ptr, &val);
       return val;
    }

    static std::valarray<double> num_dist(const AeroMode &self, 
       const BinGrid &bin_grid, const AeroData &aero_data)
    {
       int len;
       f_bin_grid_size(&bin_grid.ptr, &len);
       std::valarray<double> data(len);

       f_aero_mode_num_conc(&self.ptr, &bin_grid.ptr,
           &aero_data.ptr, begin(data), &len);

       return data; 
    }

    static void set_vol_frac(AeroMode &self, const std::valarray<double>&data)
    {
        int len = data.size();
        // Check to see they are correct length
        f_aero_mode_set_vol_frac(&self.ptr, begin(data), &len);
    }

    static std::valarray<double> get_vol_frac(const AeroMode &self)
    {
       int len;
       f_aero_mode_get_n_spec(&self.ptr, &len); 
       std::valarray<double> data(len);
       f_aero_mode_get_vol_frac(&self.ptr, begin(data), &len);
       return data;
    }
};
