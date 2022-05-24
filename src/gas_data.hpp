/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "gimmicks.hpp" // TODO: rename to something like json_resource.hpp?
#include "pmc_resource.hpp"

extern "C" void f_gas_data_ctor(void *ptr) noexcept;
extern "C" void f_gas_data_dtor(void *ptr) noexcept;
extern "C" void f_gas_data_len(const void *ptr, int *len) noexcept;
extern "C" void f_gas_data_from_json(const void *ptr) noexcept;
extern "C" void f_gas_data_to_json(const void *ptr) noexcept;

struct GasData {
    PMCResource ptr;
    const nlohmann::json json;

    GasData(const py::tuple &tpl) :
        ptr(f_gas_data_ctor, f_gas_data_dtor),
        json(tpl)
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

    static std::string __str__(const GasData &self) {
        return self.json.dump();
    }   

    static std::size_t __len__(const GasData &self) {
        int len;
        f_gas_data_len(&self.ptr, &len);
        return len;
    }   
};

