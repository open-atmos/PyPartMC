#pragma once

extern "C" void f_run_part_opt_ctor(void *ptr);
extern "C" void f_run_part_opt_dtor(void *ptr);

struct RunPartOpt {
  void *ptr;

  RunPartOpt() {
    f_run_part_opt_ctor(&this->ptr); 
  }

  ~RunPartOpt() {
    f_run_part_opt_dtor(&this->ptr);
  }
};

