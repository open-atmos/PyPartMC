/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "scenario.hpp"

double loss_rate(
    const Scenario &scenario,
    const double vol,
    const double density,
    const AeroData &aero_data,
    const EnvState &env_state
) {
    double rate;
    f_scenario_loss_rate(
        &scenario.ptr,
        &vol,
        &density,
        &aero_data.ptr,
        &env_state.ptr,
        &rate
    );
    return rate;
}

double loss_rate_dry_dep(
    const double vol,
    const double density,
    const AeroData &aero_data,
    const EnvState &env_state
) {
    double rate;
    f_scenario_loss_rate_dry_dep(
        &vol,
        &density,
        &aero_data.ptr,
        &env_state.ptr,
        &rate
    );
    return rate;
}
