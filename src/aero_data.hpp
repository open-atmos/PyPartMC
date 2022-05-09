#pragma once

extern "C" void f_aero_data_ctor(void *ptr);
extern "C" void f_aero_data_dtor(void *ptr);

struct aero_data_t {
  void *ptr;

  aero_data_t() {
    f_aero_data_ctor(&this->ptr); 
  }

  ~aero_data_t() {
    f_aero_data_dtor(&this->ptr);
  }
};

