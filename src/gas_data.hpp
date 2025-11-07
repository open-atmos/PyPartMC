/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "json_resource.hpp"
#include "pmc_resource.hpp"
#include "gas_data_parameters.hpp"
#include "camp_core.hpp"
#include "nanobind/nanobind.h"
#include "nanobind_json/nanobind_json.h"

extern "C" void f_gas_data_ctor(void *ptr) noexcept;
extern "C" void f_gas_data_dtor(void *ptr) noexcept;
extern "C" void f_gas_data_len(const void *ptr, int *len) noexcept;
extern "C" void f_gas_data_from_json(const void *ptr) noexcept;
extern "C" void f_gas_data_from_camp(const void *ptr, const void *camp_core_ptr) noexcept;
extern "C" void f_gas_data_to_json(const void *ptr) noexcept;
extern "C" void f_gas_data_spec_by_name(const void *ptr, int *value, const char *name_data,
    const int *name_size) noexcept;
extern "C" void f_gas_data_spec_name_by_index(const void *ptr, const int *i_spec,
    char *name_data) noexcept;

struct GasData {
    PMCResource ptr;
    const nlohmann::ordered_json json;

    GasData(const CampCore &CampCore) :
        ptr(f_gas_data_ctor, f_gas_data_dtor)
    {
        f_gas_data_from_camp(this->ptr.f_arg(), CampCore.ptr.f_arg());
    }

    GasData(const nanobind::tuple &tpl) :
        ptr(f_gas_data_ctor, f_gas_data_dtor),
        json(tpl)
    {
        auto json_array = nlohmann::ordered_json::array();
        for (const auto item : tpl)
            json_array.push_back(nlohmann::ordered_json::object({{
                nanobind::cast<std::string>(item),
                nlohmann::ordered_json::array()
            }}));

        JSONResourceGuard<InputJSONResource> guard(json_array);
        f_gas_data_from_json(this->ptr.f_arg());
        guard.check_parameters();
    }

    GasData() :
        ptr(f_gas_data_ctor, f_gas_data_dtor)
    {}

    static auto __str__(const GasData &self) {
        return self.json.dump();
    }   

    static std::size_t __len__(const GasData &self) {
        int len;
        f_gas_data_len(
            self.ptr.f_arg(),
            &len
        );
        return len;
    }

    static auto spec_by_name(const GasData &self, const std::string &name) {
        int value;
        const int name_size = name.size();
        f_gas_data_spec_by_name(
            self.ptr.f_arg(),
            &value,
            name.c_str(),
            &name_size
        );
        if (value == 0)
            throw std::runtime_error("Element not found.");
        return value - 1;
    }

    static auto names(const GasData &self) {

        int len;
        f_gas_data_len(
            self.ptr.f_arg(),
            &len
        );

        char name[GAS_NAME_LEN];
        nanobind::list names;
        for (int idx = 0; idx < len; idx++) {
             f_gas_data_spec_name_by_index(
                 self.ptr.f_arg(),
                 &idx,
                 name
            );
            names.append(nanobind::str(name));
        }
        return nanobind::tuple(names);
    }
};

