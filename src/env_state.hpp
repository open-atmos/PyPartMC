#pragma once

extern "C" void f_env_state_ctor(void *ptr);
extern "C" void f_env_state_dtor(void *ptr);

struct EnvState {
  void *ptr;

  EnvState() {
    f_env_state_ctor(&this->ptr); 
  }

  ~EnvState() {
    f_env_state_dtor(&this->ptr);
  }
};

