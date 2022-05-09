#pragma once

extern "C" void f_gas_data_ctor(void *ptr);
extern "C" void f_gas_data_dtor(void *ptr);

struct gas_data_t {
  void *ptr;

  gas_data_t() {
    f_gas_data_ctor(&this->ptr); 
  }

  ~gas_data_t() {
    f_gas_data_dtor(&this->ptr);
  }
};

