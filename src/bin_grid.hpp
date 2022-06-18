/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"

extern "C" void f_bin_grid_ctor(void *ptr) noexcept;
extern "C" void f_bin_grid_dtor(void *ptr) noexcept;
extern "C" void f_bin_grid_init(
    const void *ptr,
    const int *n_bin,
    const int *type,
    const double *min,
    const double *max
) noexcept;

struct BinGrid {
    PMCResource ptr;

    BinGrid(const int &n_bin, const int &type, const double &min, const double &max) :
        ptr(f_bin_grid_ctor, f_bin_grid_dtor)
    {
        f_bin_grid_init(ptr.f_arg(), &n_bin, &type, &min, &max);
    }

};
