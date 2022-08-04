/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "gimmicks.hpp"
#include "pmc_resource.hpp"

extern "C" void f_camp_core_ctor(void *ptr) noexcept;
extern "C" void f_camp_core_dtor(void *ptr) noexcept;

struct CampCore {
    PMCResource ptr;

    CampCore() :
        ptr(f_camp_core_ctor, f_camp_core_dtor)
    {
    }
};
