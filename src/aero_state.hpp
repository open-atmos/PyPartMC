#pragma once

extern "C" void f_aero_state_ctor(void *ptr);
extern "C" void f_aero_state_dtor(void *ptr);

struct AeroState {
  void *ptr;

  AeroState() {
    f_aero_state_ctor(&this->ptr); 
  }

  ~AeroState() {
    f_aero_state_dtor(&this->ptr);
  }
};

