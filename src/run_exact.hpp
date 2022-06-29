/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once
#include "bin_grid.hpp"
#include "scenario.hpp"
#include "env_state.hpp"
#include "aero_data.hpp"
#include "aero_dist_init.hpp"
#include "gas_data.hpp"
#include "run_exact_opt.hpp"

extern "C" void f_run_exact(
    const void*,
    const void*,
    const void*,
    const void*,
    const void*,
    const void*,
    const void*
) noexcept;

void run_exact(
    const Scenario &bin_grid,
    const EnvState &scenario,
    const AeroData &env_state,
    const AeroState &aero_data,
    const GasData &aero_dist_init,
    const GasState &gas_data,
    const RunExactOpt &run_exact_opt
) {
    f_run_exact(
        bin_grid.ptr.f_arg(),
        scenario.ptr.f_arg(),
        env_state.ptr.f_arg(),
        aero_data.ptr.f_arg(),
        aero_dist_init.ptr.f_arg(),
        gas_data.ptr.f_arg(),
        run_exact_opt.ptr.f_arg()
    );
}

