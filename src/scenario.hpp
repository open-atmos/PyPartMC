/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

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

