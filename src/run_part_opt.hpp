/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"

extern "C" void f_run_part_opt_ctor(void *ptr) noexcept;
extern "C" void f_run_part_opt_dtor(void *ptr) noexcept;
extern "C" void f_run_part_opt_from_json(const void *ptr) noexcept;

struct RunPartOpt {
    PMCResource ptr;

    RunPartOpt(const nlohmann::json &json) :
        ptr(f_run_part_opt_ctor, f_run_part_opt_dtor)
    {
        gimmick_ptr() = std::make_unique<InputGimmick>(json); // TODO: guard
        f_run_part_opt_from_json(this->ptr.f_arg());
        gimmick_ptr().reset();
    }
};

