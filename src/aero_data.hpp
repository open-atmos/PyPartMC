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
extern "C" void f_aero_data_len(const void *ptr, int *len) noexcept;
extern "C" void f_aero_data_set_frac_dim(void *ptr, const double*) noexcept;
extern "C" void f_aero_data_get_frac_dim(const void *ptr, double*) noexcept;
extern "C" void f_aero_data_set_vol_fill_factor(void *ptr, const double*) noexcept;
extern "C" void f_aero_data_get_vol_fill_factor(const void *ptr, double*) noexcept;
extern "C" void f_aero_data_set_prime_radius(void *ptr, const double*) noexcept;
extern "C" void f_aero_data_get_prime_radius(const void *ptr, double*) noexcept;
extern "C" void f_aero_data_rad2vol(const void *ptr, const double*, double*) noexcept;
extern "C" void f_aero_data_vol2rad(const void *ptr, const double*, double*) noexcept;
extern "C" void f_aero_data_diam2vol(const void *ptr, const double*, double*) noexcept;
extern "C" void f_aero_data_vol2diam(const void *ptr, const double*, double*) noexcept;

struct AeroData {
    PMCResource ptr;

    AeroData(const nlohmann::json &json) :
        ptr(f_aero_data_ctor, f_aero_data_dtor)
    {
        gimmick_ptr() = std::make_unique<InputGimmick>(json);
        f_aero_data_from_json(this->ptr.f_arg());
        gimmick_ptr().reset(); // TODO #117: guard
    }

    static int spec_by_name(const AeroData &self, const std::string &name) {
        int value;
        const int name_size = name.size();
        f_aero_data_spec_by_name(self.ptr.f_arg(), &value, name.c_str(), &name_size);
        if (value==0) throw std::runtime_error("Element not found.");
        return value-1;
    }

    static std::size_t __len__(const AeroData &self) {
        int len;
        f_aero_data_len(&self.ptr, &len);
        return len;
    }

    static void set_frac_dim(AeroData &self, const double value) {
        f_aero_data_set_frac_dim(&self.ptr, &value);
    }

    static double get_frac_dim(const AeroData &self) {
        double value;
        f_aero_data_get_frac_dim(&self.ptr, &value);
        return value;
    }

    static void set_vol_fill_factor(AeroData &self, const double value) {
        f_aero_data_set_vol_fill_factor(&self.ptr, &value);
    }

    static double get_prime_radius(AeroData &self) {
        double value;
        f_aero_data_get_prime_radius(&self.ptr, &value);
        return value;
    }

    static void set_prime_radius(AeroData &self, const double value) {
        f_aero_data_set_prime_radius(&self.ptr, &value);
    }

    static double get_vol_fill_factor(const AeroData &self) {
        double value;
        f_aero_data_get_vol_fill_factor(&self.ptr, &value);
        return value;
    }

    static double rad2vol(const AeroData &self, const double radius) {
        double vol;
        f_aero_data_rad2vol(&self.ptr, &radius, &vol);
        return vol;
    }

    static double vol2rad(const AeroData &self, const double vol) {
        double radius;
        f_aero_data_vol2rad(&self.ptr, &vol, &radius);
        return radius;
    }

    static double diam2vol(const AeroData &self, const double diam) {
        double vol;
        f_aero_data_diam2vol(&self.ptr, &diam, &vol);
        return vol;
    }

    static double vol2diam(const AeroData &self, const double vol) {
        double diam;
        f_aero_data_vol2diam(&self.ptr, &vol, &diam);
        return diam;
    }

};

