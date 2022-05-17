/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"

extern "C" void f_aero_state_ctor(void *ptr) noexcept;
extern "C" void f_aero_state_dtor(void *ptr) noexcept;

struct AeroState {
    PMCResource ptr;

    AeroState() :
        ptr(f_aero_state_ctor, f_aero_state_dtor)
    {
    }
};

