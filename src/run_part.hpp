#pragma once
#include "gas_data.hpp"
#include "run_part_opt.hpp"

extern "C" void f_run_part(void*, void*);

void run_part(
    const RunPartOpt &run_part_opt,
    const GasData &gas_data
) {
    f_run_part(gas_data.ptr, run_part_opt.ptr);
}

