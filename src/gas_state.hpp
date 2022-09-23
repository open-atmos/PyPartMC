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
extern "C" void f_gas_state_from_json(const void *ptr) noexcept;
extern "C" void f_gas_state_set_size(const void *ptr, const void *gasdata_ptr) noexcept;
extern "C" void f_gas_state_mix_rats(const void *ptr, const double *data, const int *len);

struct GasState {
    PMCResource ptr;

/*    GasState(const GasData &gas_data,
             const nlohmann::json &json) :
        ptr(f_gas_state_ctor, f_gas_state_dtor)
    {
        gimmick_ptr() = std::make_unique<InputGimmick>(json);

        const int n = json.empty() ? 0 : gimmick_ptr()->find("gas_mixing_ratio")->size();
        f_gas_state_set_size(this->ptr.f_arg(), &gas_data.ptr);
        //f_gas_state_set_size(this->ptr.f_arg(), &n);
        if (n != 0) f_gas_state_from_json(this->ptr.f_arg());

        gimmick_ptr().reset(); // TODO #117: guard
    }
*/

    GasState(const GasData &gas_data) :
        ptr(f_gas_state_ctor, f_gas_state_dtor)
    {
        f_gas_state_set_size(this->ptr.f_arg(), &gas_data.ptr);
    }

    static void set_item(const GasState &self, const int &idx, const double &val) {
        if (idx < 0 || idx >= (int)__len__(self))
            throw std::out_of_range("TODO #118");
        f_gas_state_set_item(&self.ptr, &idx, &val);
    }

    static double get_item(const GasState &self, const int &idx) {
        if (idx < 0 || idx >= (int)__len__(self))
            throw std::out_of_range("TODO #118");
        double value;
        f_gas_state_get_item(&self.ptr, &idx, &value);
        return value;
    }

    static std::string __str__(const GasState &self) {
        gimmick_ptr() = std::make_unique<OutputGimmick>(); // TODO #117: guard

        f_gas_state_to_json(&self.ptr);
        auto str = gimmick_ptr()->str();

        gimmick_ptr().reset(); // TODO #117: guard
        return str;
    }

    static std::size_t __len__(const GasState &self) {
        int len;
        f_gas_state_len(&self.ptr, &len);
        return len;
    }

    static double mix_rat(const GasState &self, const GasData &gasData,
         const std::string &name) {
        int value;
        const int name_size = name.size();

        f_gas_data_spec_by_name(&gasData.ptr, &value, name.c_str(), &name_size);
        if (value==0) throw std::runtime_error("Element not found.");
        return get_item(self, value-1);
    }

    static void set_size(GasState &self, const GasData &GasData) {
        f_gas_state_set_size(&self.ptr, &GasData.ptr);
    }

    static std::valarray<double> mix_rats(const GasState &self) {
        int len;
        f_gas_state_len(&self.ptr, &len);
        std::valarray<double> data(len);

        for (int idx = 0; idx < len; idx++) {
             f_gas_state_get_item(&self.ptr, &idx, &data[idx]);
        }
        return data;
    }
};
