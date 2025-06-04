/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "json_resource.hpp"
#include "pmc_resource.hpp"
#include "camp_core.hpp"

extern "C" void f_photolysis_ctor(void *ptr) noexcept;
extern "C" void f_photolysis_dtor(void *ptr) noexcept;
extern "C" void f_photolysis_create(const void *ptr, const void *camp_core) noexcept;
struct Photolysis {
    PMCResource ptr;

    Photolysis() :
        ptr(f_photolysis_ctor, f_photolysis_dtor)
    {
    }

    Photolysis(const CampCore &camp_core) :
        ptr(f_photolysis_ctor, f_photolysis_dtor)
    {
        f_photolysis_create(this->ptr.f_arg(), camp_core.ptr.f_arg());
    }
};
