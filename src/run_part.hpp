#pragma once
#include "aero_data.hpp"
#include "aero_state.hpp"
#include "env_state.hpp"
#include "gas_data.hpp"
#include "run_part_opt.hpp"
#include "scenario.hpp"

extern "C" void f_run_part(void*, void*, void*, void*, void*, void*);

void run_part(
    const Scenario &scenario,
    const EnvState &env_state,
    const AeroData &aero_data,
    const AeroState &aero_state,
    const GasData &gas_data,
    const RunPartOpt &run_part_opt
) {
    f_run_part(
        scenario.ptr,
        env_state.ptr,
        aero_data.ptr,
        aero_state.ptr,
        gas_data.ptr,
        run_part_opt.ptr
    );
}

