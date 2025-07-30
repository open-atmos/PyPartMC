/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2025 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "run_exact.hpp"

void run_exact(
    const BinGrid &bin_grid,
    const GasData &gas_data,
    const AeroData &aero_data,
    const AeroDist &aero_dist,
    const Scenario &scenario,
    const EnvState &env_state,
    const RunExactOpt &run_exact_opt
) {
    f_run_exact(
        bin_grid.ptr.f_arg(),
        gas_data.ptr.f_arg(),
        aero_data.ptr.f_arg(),
        aero_dist.ptr.f_arg(),
        scenario.ptr.f_arg(),
        env_state.ptr.f_arg(),
        run_exact_opt.ptr.f_arg()
    );
}
