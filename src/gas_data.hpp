/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once

#include "pybind11_json/pybind11_json.hpp"
#include "nlohmann/json.hpp"

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

