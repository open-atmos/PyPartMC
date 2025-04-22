/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022-2025 University of Illinois Urbana-Champaign                                  #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "aero_state.hpp"
#include "aero_binned.hpp"
#include "aero_data.hpp"
#include "aero_dist.hpp"
#include "bin_grid.hpp"
#include "env_state.hpp"
#include "gas_data.hpp"
#include "gas_state.hpp"

extern "C" void f_output_state(
    const char *prefix,
    const int *prefix_size,
    const void *aero_data,
    const void *aero_state,
    const void *gas_data,
    const void *gas_state,
    const void *env_state,
    const int *index,
    const double *time,
    const double *del_t,
    const int *i_repeat,
    const bool *record_removals,
    const bool *record_optical
) noexcept;

extern "C" void f_input_state(
    const char *filename,
    const int *filename_size,
    int *index,
    double *time,
    double *del_t,
    int *i_repeat,
    const void *aero_data,
    const void *aero_state,
    const void *gas_data,
    const void *gas_state,
    const void *env_state
) noexcept;

extern "C" void f_input_sectional(
    const char *filename,
    const int *filename_size,
    int *index,
    double *time,
    double *del_t,
    const void *bin_grid,
    const void *aero_data,
    const void *aero_binned,
    const void *gas_data,
    const void *gas_state,
    const void *env_state
) noexcept;

extern "C" void f_input_exact(
    const char *filename,
    const int *filename_size,
    int *index,
    double *time,
    double *del_t,
    const void *bin_grid,
    const void *aero_data,
    const void *aero_binned,
    const void *gas_data,
    const void *gas_state,
    const void *env_state
) noexcept;

void output_state(
    const std::string &prefix,
    const AeroData &aero_data,
    const AeroState &aero_state,
    const GasData &gas_data,
    const GasState &gas_state,
    const EnvState &env_state
);

std::tuple<std::shared_ptr<AeroData>, AeroState*, std::shared_ptr<GasData>, GasState*, EnvState*> input_state(
    const std::string &name
);

std::tuple<std::shared_ptr<AeroData>, BinGrid*, AeroBinned*, std::shared_ptr<GasData>, GasState*, EnvState*> input_sectional(
    const std::string &name
);

std::tuple<std::shared_ptr<AeroData>, BinGrid*, AeroBinned*, std::shared_ptr<GasData>, GasState*,
    EnvState*> input_exact(
    const std::string &name
);
