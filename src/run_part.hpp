/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once
#include "aero_data.hpp"
#include "aero_state.hpp"
#include "env_state.hpp"
#include "gas_data.hpp"
#include "gas_state.hpp"
#include "run_part_opt.hpp"
#include "scenario.hpp"

extern "C" void f_run_part(
    const void*,
    const void*,
    const void*,
    const void*,
    const void*,
    const void*,
    const void*
) noexcept;

void run_part(
    const Scenario &scenario,
    const EnvState &env_state,
    const AeroData &aero_data,
    const AeroState &aero_state,
    const GasData &gas_data,
    const GasState &gas_state,
    const RunPartOpt &run_part_opt
) {
    f_run_part(
        scenario.ptr.f_arg(),
        env_state.ptr.f_arg(),
        aero_data.ptr.f_arg(),
        aero_state.ptr.f_arg(),
        gas_data.ptr.f_arg(),
        gas_state.ptr.f_arg(),
        run_part_opt.ptr.f_arg()
    );
}

