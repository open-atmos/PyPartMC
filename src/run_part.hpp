/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once
#include "aero_data.hpp"
#include "aero_state.hpp"
#include "env_state.hpp"
#include "gas_data.hpp"
#include "run_part_opt.hpp"
#include "scenario.hpp"

extern "C" void f_run_part(void*, void*, void*, void*, void*, void*);

void run_part(
    const Scenario &scenario,
    const EnvState &env_state,
    const AeroData &aero_data,
    const AeroState &aero_state,
    const GasData &gas_data,
    const RunPartOpt &run_part_opt
) {
    f_run_part(
        scenario.ptr.get(),
        env_state.ptr.get(),
        aero_data.ptr.get(),
        aero_state.ptr.get(),
        gas_data.ptr.get(),
        run_part_opt.ptr.get()
    );
}

