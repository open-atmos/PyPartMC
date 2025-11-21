/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "json_resource.hpp"
#include "pmc_resource.hpp"

extern "C" void f_env_state_ctor(void *ptr) noexcept;
extern "C" void f_env_state_dtor(void *ptr) noexcept;
extern "C" void f_env_state_from_json(const void *ptr) noexcept;
extern "C" void f_env_state_set_temperature(const void *ptr, const double *temperature) noexcept;
extern "C" void f_env_state_get_temperature(const void *ptr, double *temperature) noexcept;
extern "C" void f_env_state_set_additive_kernel_coefficient(const void *ptr, const double *additive_kernel_coefficient) noexcept;
extern "C" void f_env_state_get_additive_kernel_coefficient(const void *ptr, double *additive_kernel_coefficient) noexcept;
extern "C" void f_env_state_get_rel_humid(const void *ptr, double *rel_humid) noexcept;
extern "C" void f_env_state_set_height(const void *ptr, const double *height) noexcept;
extern "C" void f_env_state_get_height(const void *ptr, double *height) noexcept;
extern "C" void f_env_state_set_pressure(const void *ptr, const double *pressure) noexcept;
extern "C" void f_env_state_get_pressure(const void *ptr, double *pressure) noexcept;
extern "C" void f_env_state_get_elapsed_time(const void *ptr, double *elapsed_time) noexcept;
extern "C" void f_env_state_get_start_time(const void *ptr, double *start_time) noexcept;
extern "C" void f_env_state_air_dens(const void *ptr, double *air_density) noexcept;
extern "C" void f_env_state_air_molar_dens(const void *ptr, double *air_molar_density) noexcept;
extern "C" void f_env_state_set_latitude(const void *ptr, const double *latitude) noexcept;
extern "C" void f_env_state_get_latitude(const void *ptr, double *latitude) noexcept;
extern "C" void f_env_state_set_longitude(const void *ptr, const double *longitude) noexcept;
extern "C" void f_env_state_get_longitude(const void *ptr, double *longitude) noexcept;
extern "C" void f_env_state_set_altitude(const void *ptr, const double *altitude) noexcept;
extern "C" void f_env_state_get_altitude(const void *ptr, double *altitude) noexcept;
extern "C" void f_env_state_ppb_to_conc(const void *ptr, const double *ppb, double *conc) noexcept;
extern "C" void f_env_state_conc_to_ppb(const void *ptr, const double *conc, double *ppb) noexcept;
extern "C" void f_env_state_sat_vapor_pressure(const void *ptr, double *sat_vapor_pressure) noexcept;

struct EnvState {
    PMCResource ptr;

    EnvState(const nlohmann::ordered_json &json) :
        ptr(f_env_state_ctor, f_env_state_dtor)
    {
        JSONResourceGuard<InputJSONResource> guard(json);
        f_env_state_from_json(this->ptr.f_arg());
        guard.check_parameters();
    }

    EnvState() :
        ptr(f_env_state_ctor, f_env_state_dtor)
    {}

    static void set_temperature(const EnvState &self, double &temperature) {
        f_env_state_set_temperature(
            self.ptr.f_arg(),
            &temperature
        );
    }

    static auto temp(const EnvState &self) {
        double temperature;

        f_env_state_get_temperature(
            self.ptr.f_arg(),
            &temperature
        );
        return temperature;
    }

    static auto rh(const EnvState &self) {
        double rel_humid;

        f_env_state_get_rel_humid(
            self.ptr.f_arg(),
            &rel_humid
        );
        return rel_humid;
    }
    
    static void set_height(const EnvState &self, const double height) {
        f_env_state_set_height(
            self.ptr.f_arg(),
            &height
        );
    }

    static auto get_height(const EnvState &self) {
        double height;

        f_env_state_get_height(
            self.ptr.f_arg(),
            &height
        );
        return height;
    }

    static void set_additive_kernel_coefficient(const EnvState &self, const double additive_kernel_coefficient) {
        f_env_state_set_additive_kernel_coefficient(
            self.ptr.f_arg(),
            &additive_kernel_coefficient
        );
    }

    static auto get_additive_kernel_coefficient(const EnvState &self) {
        double additive_kernel_coefficient;

        f_env_state_get_additive_kernel_coefficient(
            self.ptr.f_arg(),
            &additive_kernel_coefficient
        );
        return additive_kernel_coefficient;
    }

    static void set_pressure(const EnvState &self, const double pressure) {
        f_env_state_set_pressure(
            self.ptr.f_arg(),
            &pressure
        );
    }

    static auto get_pressure(const EnvState &self) {
        double pressure;

        f_env_state_get_pressure(
            self.ptr.f_arg(),
            &pressure
        );
        return pressure;
    }

    static auto get_elapsed_time(const EnvState &self) {
        double elapsed_time;

        f_env_state_get_elapsed_time(
            self.ptr.f_arg(),
            &elapsed_time
        );
        return elapsed_time;
    }

    static auto get_start_time(const EnvState &self) {
        double start_time;

        f_env_state_get_start_time(
            self.ptr.f_arg(),
            &start_time
        );
        return start_time;
    }

    static auto air_density(const EnvState &self) {
        double air_density;

        f_env_state_air_dens(
            self.ptr.f_arg(),
            &air_density
        );
        return air_density;
    }

    static auto air_molar_density(const EnvState &self) {
        double air_molar_density;

        f_env_state_air_molar_dens(
            self.ptr.f_arg(),
            &air_molar_density
        );
        return air_molar_density;
    }

    static void set_latitude(const EnvState &self, const double latitude) {
        f_env_state_set_latitude(
            self.ptr.f_arg(),
            &latitude
        );
    }

    static auto get_latitude(const EnvState &self) {
        double latitude;

        f_env_state_get_latitude(
            self.ptr.f_arg(),
            &latitude
        );
        return latitude;
    }

    static void set_longitude(const EnvState &self, const double longitude) {
        f_env_state_set_longitude(
            self.ptr.f_arg(),
            &longitude
        );
    }

    static auto get_longitude(const EnvState &self) {
        double longitude;

        f_env_state_get_longitude(
            self.ptr.f_arg(),
            &longitude
        );
        return longitude;
    }

    static void set_altitude(const EnvState &self, const double altitude) {
        f_env_state_set_altitude(
            self.ptr.f_arg(),
            &altitude
        );
    }

    static auto get_altitude(const EnvState &self) {
        double altitude;

        f_env_state_get_altitude(
            self.ptr.f_arg(),
            &altitude
        );
        return altitude;
    }

    static auto ppb_to_conc(const EnvState &self, const double ppb) {
        double conc;

        f_env_state_ppb_to_conc(
            self.ptr.f_arg(),
            &ppb,
            &conc
        );
        return conc;
    }

    static auto conc_to_ppb(const EnvState &self, const double conc) {
        double ppb;

        f_env_state_conc_to_ppb(
            self.ptr.f_arg(),
            &conc,
            &ppb
        );
        return ppb;
    }

    static auto sat_vapor_pressure(const EnvState &self) {
        double sat_vapor_pressure;

        f_env_state_sat_vapor_pressure(
            self.ptr.f_arg(),
            &sat_vapor_pressure
        );
        return sat_vapor_pressure;
    }

};
