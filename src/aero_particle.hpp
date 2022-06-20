/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"

extern "C" void f_aero_particle_ctor(void *ptr) noexcept;
extern "C" void f_aero_particle_dtor(void *ptr) noexcept;

struct AeroParticle {
    PMCResource ptr;

    AeroParticle() :
        ptr(f_aero_particle_ctor, f_aero_particle_dtor)
    {
    }
};

