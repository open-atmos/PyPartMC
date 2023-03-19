/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "run_part.hpp"

void run_part(
    const Scenario &scenario,
    EnvState &env_state,
    const AeroData &aero_data,
    AeroState &aero_state,
    const GasData &gas_data,
    GasState &gas_state,
    const RunPartOpt &run_part_opt,
    const CampCore &camp_core,
    const Photolysis &photolysis
) {
    f_run_part(
        scenario.ptr.f_arg(),
        env_state.ptr.f_arg_non_const(),
        aero_data.ptr.f_arg(),
        aero_state.ptr.f_arg_non_const(),
        gas_data.ptr.f_arg(),
        gas_state.ptr.f_arg_non_const(),
        run_part_opt.ptr.f_arg(),
        camp_core.ptr.f_arg(),
        photolysis.ptr.f_arg()
    );
}

void run_part_timestep(
    const Scenario &scenario,
    EnvState &env_state,
    const AeroData &aero_data,
    AeroState &aero_state,
    const GasData &gas_data,
    GasState &gas_state,
    const RunPartOpt &run_part_opt,
    const CampCore &camp_core,
    const Photolysis &photolysis,
    const int &i_time,
    const double &t_start
) {
    f_run_part_timestep(
        scenario.ptr.f_arg(),
        env_state.ptr.f_arg_non_const(),
        aero_data.ptr.f_arg(),
        aero_state.ptr.f_arg_non_const(),
        gas_data.ptr.f_arg(),
        gas_state.ptr.f_arg_non_const(),
        run_part_opt.ptr.f_arg(),
        camp_core.ptr.f_arg(),
        photolysis.ptr.f_arg(),
        &i_time,
        &t_start
    );
}

void run_part_timeblock(
    const Scenario &scenario,
    EnvState &env_state,
    const AeroData &aero_data,
    AeroState &aero_state,
    const GasData &gas_data,
    GasState &gas_state,
    const RunPartOpt &run_part_opt,
    const CampCore &camp_core,
    const Photolysis &photolysis,
    const int &i_time,
    const int &i_next,
    const double &t_start
) {
    f_run_part_timeblock(
        scenario.ptr.f_arg(),
        env_state.ptr.f_arg_non_const(),
        aero_data.ptr.f_arg(),
        aero_state.ptr.f_arg_non_const(),
        gas_data.ptr.f_arg(),
        gas_state.ptr.f_arg_non_const(),
        run_part_opt.ptr.f_arg(),
        camp_core.ptr.f_arg(),
        photolysis.ptr.f_arg(),
        &i_time,
        &i_next,
        &t_start
    );
}
