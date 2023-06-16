/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "aero_state.hpp"
#include "aero_data.hpp"
#include "aero_dist.hpp"
#include "env_state.hpp"
#include "gas_data.hpp"
#include "gas_state.hpp"

extern "C" void f_output_state(
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

void output_state(
    const AeroData &aero_data,
    const AeroState &aero_state,
    const GasData &gas_data,
    const GasState &gas_state,
    const EnvState &env_state
);
