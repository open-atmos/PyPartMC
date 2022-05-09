#pragma once

extern "C" void f_run_part_opt_ctor(void *ptr);
extern "C" void f_run_part_opt_dtor(void *ptr);

struct run_part_opt_t {
  void *ptr;

  run_part_opt_t() {
    f_run_part_opt_ctor(&this->ptr); 
  }

  ~run_part_opt_t() {
    f_run_part_opt_dtor(&this->ptr);
  }
};

