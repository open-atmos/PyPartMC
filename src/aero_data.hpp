#pragma once

extern "C" void f_aero_data_ctor(void *ptr);
extern "C" void f_aero_data_dtor(void *ptr);

struct AeroData {
  void *ptr;

  AeroData() {
    f_aero_data_ctor(&this->ptr); 
  }

  ~AeroData() {
    f_aero_data_dtor(&this->ptr);
  }
};

