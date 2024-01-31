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
#include "camp_core.hpp"
#include "photolysis.hpp"

extern "C" void f_run_part(
    const void*,
    const void*,
    const void*,
    const void*,
    const void*,
    const void*,
    const void*,
    const void*,
    const void*
) noexcept;

extern "C" void f_run_part_timestep(
    const void*,
    void*,
    const void*,
    void*,
    const void*,
    void*,
    const void*,
    const void*,
    const void*,
    const int *,
    const double*,
    double*,
    double*,
    int*
) noexcept;

extern "C" void f_run_part_timeblock(
    const void*,
    void*,
    const void*,
    void*,
    const void*,
    void*,
    const void*,
    const void*,
    const void*,
    const int *,
    const int *,
    const double *,
    double*,
    double*,
    int*
) noexcept;

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
);

std::tuple<double, double, int> run_part_timestep(
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
    const double &t_start,
    double &last_output_time,
    double &last_progress_time,
    int &i_output
);

std::tuple<double, double, int> run_part_timeblock(
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
    const double &t_start,
    double &last_output_time,
    double &last_progress_time,
    int &i_output
);
