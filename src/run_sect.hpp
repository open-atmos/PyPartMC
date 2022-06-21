/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once
#include "bin_grid.hpp"
#include "gas_data.hpp"
#include "aero_data.hpp"
#include "aero_dist.hpp"
#include "scenario.hpp"
#include "env_state.hpp"
#include "run_sect_opt.hpp"

extern "C" void f_run_sect(
    const void*,
    const void*,
    const void*,
    const void*,
    const void*,
    const void*,
    const void*
) noexcept;

void run_sect(
    const BinGrid &bin_grid,
    const GasData &gas_data,
    const AeroData &aero_data,
    const AeroDist &aero_dist,
    const Scenario &scenario,
    const EnvState &env_state,
    const RunSectOpt &run_sect_opt
) {
    f_run_sect(
        bin_grid.ptr.f_arg(),
        gas_data.ptr.f_arg(),
        aero_data.ptr.f_arg(),
        aero_dist.ptr.f_arg(),
        scenario.ptr.f_arg(),
        env_state.ptr.f_arg(),
        run_sect_opt.ptr.f_arg()
    );
}

