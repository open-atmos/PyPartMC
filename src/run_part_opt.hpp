/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"

extern "C" void f_run_part_opt_ctor(void *ptr) noexcept;
extern "C" void f_run_part_opt_dtor(void *ptr) noexcept;

struct RunPartOpt {
    PMCResource ptr;

    RunPartOpt(const nlohmann::json &json) :
        ptr(f_run_part_opt_ctor, f_run_part_opt_dtor)
    {
    }
};

