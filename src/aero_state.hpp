/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "aero_data.hpp"
#include "pybind11/stl.h"

extern "C" void f_aero_state_ctor(void *ptr) noexcept;
extern "C" void f_aero_state_dtor(void *ptr) noexcept;
extern "C" void f_aero_state_init(
    const void *ptr,
    const double *n_part,
    const void *aero_dataptr
) noexcept;
extern "C" void f_aero_state_len(const void *ptr, int *len) noexcept;
extern "C" void f_aero_state_total_num_conc(const void *ptr, const void *aero_dataptr,
    double *total_num_conc) noexcept;
extern "C" void f_aero_state_num_concs(const void *ptr, const void *aero_dataptr, 
    double *num_concs, const int *len) noexcept;

struct AeroState {
    PMCResource ptr;

    AeroState(const double &n_part, const AeroData &aero_data) :
        ptr(f_aero_state_ctor, f_aero_state_dtor)
    {
        f_aero_state_init(ptr.f_arg(), &n_part, aero_data.ptr.f_arg());
    }

    static std::size_t __len__(const AeroState &self) {
        int len;
        f_aero_state_len(&self.ptr, &len);
        return len;
    }

    static double total_num_conc(const AeroState &self, const AeroData &aero_data){
        double total_num_conc;
        f_aero_state_total_num_conc(&self.ptr, &aero_data.ptr, &total_num_conc);
        return total_num_conc;
    }

    static std::valarray<double> num_concs(const AeroState &self, const AeroData &aero_data){
        int len;
        f_aero_state_len(&self.ptr, &len);
        std::valarray<double> num_concs(len);

        f_aero_state_num_concs(&self.ptr, &aero_data.ptr, begin(num_concs), &len);

        return num_concs;
    }
};
