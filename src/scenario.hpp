/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once
#include <string>
#include "pybind11_json/pybind11_json.hpp"
#include "nlohmann/json.hpp"
#include "gimmicks.hpp"


extern "C" void f_scenario_ctor(void *ptr);
extern "C" void f_scenario_dtor(void *ptr);
extern "C" void f_scenario_from_json(void *ptr);

struct Scenario {
    void *ptr; // TODO: common base class

    Scenario(const nlohmann::json &json) {
        f_scenario_ctor(&this->ptr); 

        gimmick_ptr() = std::make_unique<InputGimmick>(json); // TODO: guard
        f_scenario_from_json(&this->ptr);
        gimmick_ptr().reset();
    }

    ~Scenario() {
        f_scenario_dtor(&this->ptr);
    }
};

