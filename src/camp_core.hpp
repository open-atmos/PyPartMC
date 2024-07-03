/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "json_resource.hpp"
#include "pmc_resource.hpp"

extern "C" void f_camp_core_ctor(void *ptr) noexcept;
extern "C" void f_camp_core_dtor(void *ptr) noexcept;
extern "C" void f_camp_core_initialize(void *ptr, const char *prefix,
    const int *prefix_size) noexcept;

struct CampCore {
    PMCResource ptr;

    CampCore() :
        ptr(f_camp_core_ctor, f_camp_core_dtor)
    {
    }

    CampCore(const std::string &config_name) :
        ptr(f_camp_core_ctor, f_camp_core_dtor)
    {
        const int config_name_size = config_name.size();
        f_camp_core_initialize(ptr.f_arg_non_const(), config_name.c_str(), &config_name_size);
    }
};
