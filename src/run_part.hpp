#pragma once
#include "gas_data.hpp"

struct run_part_opt_t {
  double t_max, t_output;
};

extern "C" void f_run_part(run_part_opt_t, void*);

void run_part(
    const run_part_opt_t &run_part_opt,
    const gas_data_t &gas_data
) {
    f_run_part(run_part_opt, gas_data.ptr);
}

