/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"

extern "C" void f_aero_dist_ctor(
    void *ptr
) noexcept;

extern "C" void f_aero_dist_dtor(
    void *ptr
) noexcept;

extern "C" void f_aero_dist_from_json(
    void *ptr,
    void *aero_data_ptr
) noexcept;

struct AeroDist {
    PMCResource ptr;

    AeroDist(AeroData &aero_data, const nlohmann::json &json):
        ptr(f_aero_dist_ctor, f_aero_dist_dtor)
    {
        gimmick_ptr() = std::make_unique<InputGimmick>(json, "", "mode_name", 1);
        f_aero_dist_from_json(ptr.f_arg_non_const(), aero_data.ptr.f_arg_non_const());
        gimmick_ptr().reset();
    }
};
