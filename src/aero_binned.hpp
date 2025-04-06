/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "aero_data.hpp"
#include "bin_grid.hpp"
#include "aero_dist.hpp"
#include "pybind11/stl.h"

extern "C" void f_aero_binned_ctor(void *ptr) noexcept;
extern "C" void f_aero_binned_dtor(void *ptr) noexcept;
extern "C" void f_aero_binned_len(
    const void *ptr, int *len
) noexcept;
extern "C" void f_aero_binned_add_aero_dist(
    void *ptr,
    const void *aero_data,
    const void *bin_grid,
    const void *aero_dist
) noexcept;
extern "C" void f_aero_binned_set_sizes(
    void *ptr,
    const void *aero_data,
    const void *aero_binned
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
        ptr(f_aero_binned_ctor, f_aero_binned_dtor),
        aero_data(aero_data)
    {
    }

    AeroBinned(std::shared_ptr<AeroData> aero_data, const BinGrid &bin_grid) : 
        ptr(f_aero_binned_ctor, f_aero_binned_dtor),
        aero_data(aero_data)
    {
        f_aero_binned_set_sizes(
            ptr.f_arg_non_const(),
            aero_data->ptr.f_arg(),
            bin_grid.ptr.f_arg()
        );
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

    static void add_aero_dist(AeroBinned &self, 
        const BinGrid &bin_grid, const AeroDist &aero_dist)
    {
        f_aero_binned_add_aero_dist(
            self.ptr.f_arg_non_const(),
            bin_grid.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            aero_dist.ptr.f_arg()
        );
    }

};

