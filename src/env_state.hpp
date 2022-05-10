/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

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

