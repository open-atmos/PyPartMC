/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"

extern "C" void f_aero_state_ctor(void *ptr) noexcept;
extern "C" void f_aero_state_dtor(void *ptr) noexcept;
extern "C" void f_aero_state_set_n_part_ideal(const void *ptr, const double *n_part) noexcept;


struct AeroState {
    PMCResource ptr;

    AeroState(const double &n_part) :
        ptr(f_aero_state_ctor, f_aero_state_dtor)
    {
        f_aero_state_set_n_part_ideal(ptr.f_arg(), &n_part);
    }
};

