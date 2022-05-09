#pragma once

extern "C" void f_gas_data_ctor(void *ptr);
extern "C" void f_gas_data_dtor(void *ptr);

struct GasData {
  void *ptr;

  GasData() {
    f_gas_data_ctor(&this->ptr); 
  }

  ~GasData() {
    f_gas_data_dtor(&this->ptr);
  }
};

