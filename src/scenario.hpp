/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "gimmicks.hpp"
#include "pmc_resource.hpp"
#include "aero_data.hpp"
#include "env_state.hpp"
#include "gas_data.hpp"

extern "C" void f_scenario_ctor(void *ptr) noexcept;
extern "C" void f_scenario_dtor(void *ptr) noexcept;
extern "C" void f_scenario_from_json(
    const void *gas_data,
    const void *aero_data,
    const void *scenario
)
#ifndef __linux__
    noexcept
#endif
;
extern "C" void f_scenario_loss_rate(
    const void *scenario,
    const double *vol,
    const double *density,
    const void *aero_data,
    const void *env_state,
    double *rate
) noexcept;
extern "C" void f_scenario_loss_rate_dry_dep(
    const double *vol,
    const double *density,
    const void *aero_data,
    const void *env_state,
    double *rate
) noexcept;
extern "C" void f_scenario_init_env_state(
    const void *scenario,
    void *env_state,
    const double *time
) noexcept;

struct Scenario {
    PMCResource ptr;
    const nlohmann::json json;

    Scenario(
        const GasData &gas_data,
        const AeroData &aero_data,
        const nlohmann::json &json
    ) :
        ptr(f_scenario_ctor, f_scenario_dtor),
        json(json)
    {
        gimmick_ptr() = std::make_unique<InputGimmick>(json, "dist", "mode_name"); // TODO #117: guard
        f_scenario_from_json(
            gas_data.ptr.f_arg(),
            aero_data.ptr.f_arg(),
            this->ptr.f_arg()
        );
        gimmick_ptr().reset();
    }

    static auto __str__(const Scenario &self) {
        return self.json.dump();
    }   

static void init_env_state(
    const Scenario &self,
    EnvState &env_state,
    const double time
) {
    f_scenario_init_env_state(
         self.ptr.f_arg(),
         env_state.ptr.f_arg_non_const(),
         &time
    );
}

};

double loss_rate(
    const Scenario &scenario,
    const double vol,
    const double density,
    const AeroData &aero_data,
    const EnvState &env_state
);

double loss_rate_dry_dep(
    const double vol,
    const double density,
    const AeroData &aero_data,
    const EnvState &env_state
);
