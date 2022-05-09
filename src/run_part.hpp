#pragma once
#include "aero_data.hpp"
#include "gas_data.hpp"
#include "run_part_opt.hpp"
#include "scenario.hpp"

extern "C" void f_run_part(void*, void*, void*, void*);

void run_part(
    const Scenario &scenario,
    const AeroData &aero_data,
    const GasData &gas_data,
    const RunPartOpt &run_part_opt
) {
    f_run_part(
        scenario.ptr,
        aero_data.ptr,
        gas_data.ptr,
        run_part_opt.ptr
    );
}

