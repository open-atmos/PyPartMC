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
    const void *ptr
) noexcept;
extern "C" void f_aero_mode_total_num_conc(
    const void *ptr,
    const double *val
) noexcept;

struct AeroMode {
    PMCResource ptr;

    AeroMode() :
        ptr(f_bin_grid_ctor, f_bin_grid_dtor)
    {
        f_aero_mode_init(ptr.f_arg());
    }

    static double num_conc(const AeroMode &self){
       double val;
       f_aero_mode_total_num_conc(&self.ptr, &val);
       return val;
    }
};
