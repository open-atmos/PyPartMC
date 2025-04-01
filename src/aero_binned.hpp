/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "aero_data.hpp"
#include "pybind11/stl.h"

extern "C" void f_bin_grid_ctor(void *ptr) noexcept;
extern "C" void f_bin_grid_dtor(void *ptr) noexcept;
extern "C" void f_aero_binned_len(
    const void *ptr, int *len
) noexcept;

extern "C" void f_aero_binned_num_conc(
    const void *ptr,
    double *num_conc,
    const int *len
) noexcept;

struct AeroBinned {
    PMCResource ptr;
    std::shared_ptr<AeroData> aero_data;
    AeroBinned(std::shared_ptr<AeroData> aero_data) : 
        ptr(f_bin_grid_ctor, f_bin_grid_dtor),
        aero_data(aero_data)
    {
    }

    static auto num_conc(const AeroBinned &self) {
        int len;
        f_aero_binned_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> num_conc(len);

        f_aero_binned_num_conc(
            self.ptr.f_arg(),
            begin(num_conc),
            &len
        );

        return num_conc;
    }

};

