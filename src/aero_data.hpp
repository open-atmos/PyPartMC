/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "gimmicks.hpp"
#include "aero_data_parameters.hpp"

extern "C" void f_aero_data_ctor(void *ptr) noexcept;
extern "C" void f_aero_data_dtor(void *ptr) noexcept;
extern "C" void f_aero_data_from_json(const void *ptr) noexcept;
extern "C" void f_aero_data_spec_by_name(const void *ptr, int *value, const char *name_data, const int *name_size) noexcept;
extern "C" void f_aero_data_len(const void *ptr, int *len) noexcept;
extern "C" void f_aero_data_n_source(const void *ptr, int *len) noexcept;
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
extern "C" void f_aero_data_get_species_density(const void *ptr, const int *idx, double *val) noexcept;

struct AeroData {
    PMCResource ptr;

    AeroData(const nlohmann::json &json) :
        ptr(f_aero_data_ctor, f_aero_data_dtor)
    {
        if (!InputGimmick::unique_keys(json))
            throw std::runtime_error("Species names must be unique");

        GimmickGuard<InputGimmick> guard(json);
        f_aero_data_from_json(this->ptr.f_arg());
    }

    static auto spec_by_name(const AeroData &self, const std::string &name) {
        int value;
        const int name_size = name.size();
        f_aero_data_spec_by_name(
            self.ptr.f_arg(),
            &value,
            name.c_str(),
            &name_size
        );
        if (value==0)
            throw std::runtime_error("Element not found.");
        return value - 1;
    }

    static std::size_t __len__(const AeroData &self) {
        int len;
        f_aero_data_len(
            self.ptr.f_arg(),
            &len
        );
        return len;
    }

    static void set_frac_dim(AeroData &self, const double value) {
        f_aero_data_set_frac_dim( 
            self.ptr.f_arg_non_const(),
            &value
        );
    }

    static auto get_frac_dim(const AeroData &self) {
        double value;
        f_aero_data_get_frac_dim(
            self.ptr.f_arg(),
            &value
        );
        return value;
    }

    static void set_vol_fill_factor(AeroData &self, const double value) {
        f_aero_data_set_vol_fill_factor(
            self.ptr.f_arg_non_const(),
            &value
        );
    }

    static auto get_prime_radius(AeroData &self) {
        double value;
        f_aero_data_get_prime_radius(
            self.ptr.f_arg(),
            &value
        );
        return value;
    }

    static void set_prime_radius(AeroData &self, const double value) {
        f_aero_data_set_prime_radius(
            self.ptr.f_arg_non_const(),
            &value
        );
    }

    static auto get_vol_fill_factor(const AeroData &self) {
        double value;
        f_aero_data_get_vol_fill_factor(
            self.ptr.f_arg(),
            &value
        );
        return value;
    }

    static auto rad2vol(const AeroData &self, const double radius) {
        double vol;
        f_aero_data_rad2vol(
            self.ptr.f_arg(),
            &radius,
            &vol
        );
        return vol;
    }

    static auto vol2rad(const AeroData &self, const double vol) {
        double radius;
        f_aero_data_vol2rad(
            self.ptr.f_arg(),
            &vol,
            &radius
        );
        return radius;
    }

    static auto diam2vol(const AeroData &self, const double diam) {
        double vol;
        f_aero_data_diam2vol(
            self.ptr.f_arg(),
            &diam,
            &vol
        );
        return vol;
    }

    static auto vol2diam(const AeroData &self, const double vol) {
        double diam;
        f_aero_data_vol2diam(
            self.ptr.f_arg(),
            &vol,
            &diam
        );
        return diam;
    }

    static auto densities(const AeroData &self) {
        int len;
        f_aero_data_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> data(len);

        for (int idx = 0; idx < len; idx++) {
             f_aero_data_get_species_density(
                 self.ptr.f_arg(),
                 &idx,
                 &data[idx]
            );
        }
        return data;
    }

    static auto density(const AeroData &self, const std::string &name) {
        int idx;
        double data;
        const int name_size = name.size();

        f_aero_data_spec_by_name(
            self.ptr.f_arg(),
            &idx,
            name.c_str(),
            &name_size
        );

        if (idx==0)
            throw std::runtime_error("Element not found.");

        idx--;
        f_aero_data_get_species_density(
            self.ptr.f_arg(),
            &idx,
            &data
        );

        return data;
    }

    static std::size_t n_source(const AeroData &self) {
        int len;
        f_aero_data_n_source(
            self.ptr.f_arg(),
            &len
        );
        if (len == -1)
            throw std::runtime_error("No sources defined.");
        return len;
    }

};

