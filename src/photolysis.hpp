/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "gimmicks.hpp"
#include "pmc_resource.hpp"

extern "C" void f_photolysis_ctor(void *ptr) noexcept;
extern "C" void f_photolysis_dtor(void *ptr) noexcept;

struct Photolysis {
    PMCResource ptr;

    Photolysis() :
        ptr(f_photolysis_ctor, f_photolysis_dtor)
    {
    }
};
