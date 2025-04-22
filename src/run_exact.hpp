/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2025 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once
#include "aero_data.hpp"
#include "bin_grid.hpp"
#include "env_state.hpp"
#include "gas_data.hpp"
#include "gas_state.hpp"
#include "run_exact_opt.hpp"
#include "scenario.hpp"

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
    const BinGrid &bin_grid,
    const GasData &gas_data,
    const AeroData &aero_data,
    const AeroDist &aero_dist,
    const Scenario &scenario,
    const EnvState &env_state,
    const RunExactOpt &run_exact_opt
);
