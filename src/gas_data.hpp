/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once

#include "pybind11_json/pybind11_json.hpp"
#include "nlohmann/json.hpp"

#include "pmc_resource.hpp"

extern "C" void f_gas_data_ctor(void *ptr) noexcept;
extern "C" void f_gas_data_dtor(void *ptr) noexcept;

struct GasData {
    PMCResource ptr;

    GasData() :
        ptr(f_gas_data_ctor, f_gas_data_dtor)
    {
    }
};

