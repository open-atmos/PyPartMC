/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

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

