/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once
#include <string>
#include "pybind11_json/pybind11_json.hpp"
#include "nlohmann/json.hpp"
#include "gimmicks.hpp"
#include "pmc_resource.hpp"
#include "gas_data.hpp"

extern "C" void f_gas_state_ctor(void *ptr) noexcept;
extern "C" void f_gas_state_dtor(void *ptr) noexcept;
extern "C" void f_gas_state_set_item(const void *ptr, const int *idx, const double *val) noexcept;
extern "C" void f_gas_state_get_item(const void *ptr, const int *idx, double *val) noexcept;
extern "C" void f_gas_state_len(const void *ptr, int *len) noexcept;
extern "C" void f_gas_state_to_json(const void *ptr) noexcept;
extern "C" void f_gas_state_from_json(const void *ptr, const void *gasdata_ptr) noexcept;
extern "C" void f_gas_state_set_size(const void *ptr, const void *gasdata_ptr) noexcept;
extern "C" void f_gas_state_mix_rats(const void *ptr, const double *data, const int *len);

struct GasState {
    PMCResource ptr;
    std::shared_ptr<GasData> gas_data;

    GasState(std::shared_ptr<GasData> gas_data):
        ptr(f_gas_state_ctor, f_gas_state_dtor),
        gas_data(gas_data)
    {
        f_gas_state_set_size(
            this->ptr.f_arg(),
            gas_data->ptr.f_arg()
        );
    }

    static void set_item(const GasState &self, const int &idx, const double &val) {
        if (idx < 0 || idx >= (int)__len__(self))
            throw std::out_of_range("TODO #118");
        f_gas_state_set_item(
            self.ptr.f_arg(),
            &idx,
            &val
        );
    }

    static auto get_item(const GasState &self, const int &idx) {
        if (idx < 0 || idx >= (int)__len__(self))
            throw std::out_of_range("TODO #118");
        double value;
        f_gas_state_get_item(
            self.ptr.f_arg(),
            &idx,
            &value
        );
        return value;
    }

    static auto __str__(const GasState &self) {
        GimmickGuard<OutputGimmick> guard;

        f_gas_state_to_json(self.ptr.f_arg());
        auto str = gimmick_ptr()->str();

        return str;
    }

    static std::size_t __len__(const GasState &self) {
        int len;
        f_gas_state_len(self.ptr.f_arg(), &len);
        return len;
    }

    static auto mix_rat(
        const GasState &self,
        const std::string &name
    ) {
        int value;
        const int name_size = name.size();

        f_gas_data_spec_by_name(
            self.gas_data->ptr.f_arg(),
            &value,
            name.c_str(),
            &name_size
        );
        if (value == 0)
            throw std::runtime_error("Element not found.");
        return get_item(self, value - 1);
    }

    static void set_size(GasState &self) {
        f_gas_state_set_size(
            self.ptr.f_arg(),
            self.gas_data->ptr.f_arg()
        );
    }

    static auto mix_rats(const GasState &self) {
        int len;
        f_gas_state_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> data(len);

        for (int idx = 0; idx < len; idx++) {
             f_gas_state_get_item(
                 self.ptr.f_arg(),
                 &idx,
                 &data[idx]
             );
        }
        return data;
    }

    static void set_mix_rats(const GasState &self, const nlohmann::json &json) {
        GimmickGuard<InputGimmick> guard(json);
        f_gas_state_from_json(self.ptr.f_arg(),
             self.gas_data->ptr.f_arg());
    }
};
