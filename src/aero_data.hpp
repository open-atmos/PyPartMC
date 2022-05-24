/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "gimmicks.hpp"

extern "C" void f_aero_data_ctor(void *ptr) noexcept;
extern "C" void f_aero_data_dtor(void *ptr) noexcept;
extern "C" void f_aero_data_from_json(const void *ptr) noexcept;

struct AeroData {
    PMCResource ptr;

    AeroData(const nlohmann::json &json) :
        ptr(f_aero_data_ctor, f_aero_data_dtor)
    {
        gimmick_ptr() = std::make_unique<InputGimmick>(json);
        f_aero_data_from_json(this->ptr.f_arg());
        gimmick_ptr().reset(); // TODO: guard
    }
};

