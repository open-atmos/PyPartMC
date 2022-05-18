/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once

#include "gimmicks.hpp"
#include "pmc_resource.hpp"
#include "aero_data.hpp"
#include "gas_data.hpp"

extern "C" void f_scenario_ctor(void *ptr) noexcept;
extern "C" void f_scenario_dtor(void *ptr) noexcept;
extern "C" void f_scenario_from_json(
  const void *gas_data,
  const void *aero_data,
  const void *scenario
);// noexcept;

struct Scenario {
    PMCResource ptr;

    Scenario(
        const GasData &gas_data,
        const AeroData &aero_data,
        const nlohmann::json &json
    ) :
        ptr(f_scenario_ctor, f_scenario_dtor)
    {
        gimmick_ptr() = std::make_unique<InputGimmick>(json); // TODO: guard
        f_scenario_from_json(
            gas_data.ptr.f_arg(),
            aero_data.ptr.f_arg(),
            this->ptr.f_arg()
        );
        gimmick_ptr().reset();
    }
};

