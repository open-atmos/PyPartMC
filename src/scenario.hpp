#pragma once

extern "C" void f_scenario_ctor(void *ptr);
extern "C" void f_scenario_dtor(void *ptr);

struct Scenario {
  void *ptr;

  Scenario() {
    f_scenario_ctor(&this->ptr); 
  }

  ~Scenario() {
    f_scenario_dtor(&this->ptr);
  }
};

