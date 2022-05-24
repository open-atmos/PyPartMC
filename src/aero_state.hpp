/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "aero_data.hpp"

extern "C" void f_aero_state_ctor(void *ptr) noexcept;
extern "C" void f_aero_state_dtor(void *ptr) noexcept;
extern "C" void f_aero_state_init(
    const void *ptr,
    const double *n_part,
    const void *aero_dataptr
) noexcept;
extern "C" void f_aero_state_len(const void *ptr, int *len) noexcept;

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
};

