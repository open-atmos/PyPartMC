/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

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

