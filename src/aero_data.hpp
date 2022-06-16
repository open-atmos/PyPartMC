/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "gimmicks.hpp"

extern "C" void f_aero_data_ctor(void *ptr) noexcept;
extern "C" void f_aero_data_dtor(void *ptr) noexcept;
extern "C" void f_aero_data_from_json(const void *ptr) noexcept;
extern "C" void f_aero_data_spec_by_name(const void *ptr, int *value, const char *name_data, const int *name_size) noexcept;

struct AeroData {
    PMCResource ptr;

    AeroData(const nlohmann::json &json) :
        ptr(f_aero_data_ctor, f_aero_data_dtor)
    {
        gimmick_ptr() = std::make_unique<InputGimmick>(json);
        f_aero_data_from_json(this->ptr.f_arg());
        gimmick_ptr().reset(); // TODO: guard
    }

    static int spec_by_name(const AeroData &self, const std::string &name) {
        int value;
        const int name_size = name.size();
        f_aero_data_spec_by_name(self.ptr.f_arg(), &value, name.c_str(), &name_size);
        if (value==0) throw std::runtime_error("Element not found.");
        return value-1;
    }
};

