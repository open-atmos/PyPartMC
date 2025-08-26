/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2025 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "run_sect.hpp"

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
