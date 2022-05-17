/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once
#include <iostream>
#include <string>
#include "pybind11_json/pybind11_json.hpp"
#include "nlohmann/json.hpp"
#include "gimmicks.hpp"
#include "pmc_resource.hpp"

extern "C" void f_scenario_ctor(void *ptr) noexcept;
extern "C" void f_scenario_dtor(void *ptr) noexcept;
extern "C" void f_scenario_from_json(void *ptr) noexcept;

struct Scenario {
    PMCResource ptr;

    Scenario(const nlohmann::json &json) :
        ptr(f_scenario_ctor, f_scenario_dtor)
    {
        gimmick_ptr() = std::make_unique<InputGimmick>(json); // TODO: guard
        f_scenario_from_json(this->ptr.get());
        gimmick_ptr().reset();
    }
};

