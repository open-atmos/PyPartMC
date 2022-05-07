#pragma once

struct run_part_opt_t {
  double t_max, t_output;
};

extern "C" void py_run_part(run_part_opt_t);

void run_part(const run_part_opt_t &run_part_opt) {
    py_run_part(run_part_opt);
}

