/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once

#include "gimmicks.hpp" // TODO: rename to something like json_resource.hpp?
#include "pmc_resource.hpp"

extern "C" void f_gas_data_ctor(void *ptr) noexcept;
extern "C" void f_gas_data_dtor(void *ptr) noexcept;
extern "C" void f_gas_data_from_json(const void *ptr) noexcept;

struct GasData {
    PMCResource ptr;

    GasData(const py::tuple &tpl) :
        ptr(f_gas_data_ctor, f_gas_data_dtor)
    {
        auto json_array = nlohmann::json::array();
        for (const auto &item : tpl)
            json_array.push_back(nlohmann::json::object({{
                item.cast<std::string>(),
                nlohmann::json::array()
            }}));

        gimmick_ptr() = std::make_unique<InputGimmick>(json_array);
        f_gas_data_from_json(this->ptr.f_arg());
        gimmick_ptr().reset(); // TODO: guard
    }
};

