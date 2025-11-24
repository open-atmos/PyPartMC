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

    template <typename T = double, typename Func>
    static T get_value(const EnvState &self, Func func) {
        T value{};
        func(self.ptr.f_arg(), &value);
        return value;
    }

    template <typename T = double, typename Func>
    static void set_value(const EnvState &self, Func func, T value) {
        func(self.ptr.f_arg(), &value);
    }

    static double temp(const EnvState &self) {
        return get_value(self, f_env_state_get_temperature);
    }

    static void set_temperature(const EnvState &self, double temperature) {
        set_value(self, f_env_state_set_temperature, temperature);
    }

    static auto rh(const EnvState &self) {
        return get_value(self, f_env_state_get_rel_humid);
    }

    static void set_height(const EnvState &self, const double height) {
        set_value(self, f_env_state_set_height, height);
    }

    static double get_height(const EnvState &self) {
        return get_value(self, f_env_state_get_height);
    }

    static void set_additive_kernel_coefficient(
        const EnvState &self,
        const double additive_kernel_coefficient)
    {
        set_value(
            self,
            f_env_state_set_additive_kernel_coefficient,
            additive_kernel_coefficient);
    }
    static double get_additive_kernel_coefficient(const EnvState &self) {
        return get_value(self, f_env_state_get_additive_kernel_coefficient);
    }

    static void set_pressure(const EnvState &self, const double pressure) {
        set_value(self, f_env_state_set_pressure, pressure);
    }

    static double get_pressure(const EnvState &self) {
        return get_value(self, f_env_state_get_pressure);
    }

    static auto get_elapsed_time(const EnvState &self) {
        return get_value(self, f_env_state_get_elapsed_time);
    }

    static auto get_start_time(const EnvState &self) {
        return get_value(self, f_env_state_get_start_time);
    }

    static auto air_density(const EnvState &self) {
        return get_value(self, f_env_state_air_dens);
    }

    static auto air_molar_density(const EnvState &self) {
        return get_value(self, f_env_state_air_molar_dens);
    }

    static auto get_latitude(const EnvState &self) {
        return get_value(self, f_env_state_get_latitude);
    }

    static void set_latitude(const EnvState &self, const double latitude) {
        set_value(self, f_env_state_set_latitude, latitude);
    }

    static auto get_longitude(const EnvState &self) {
        return get_value(self, f_env_state_get_longitude);
    }

    static void set_longitude(const EnvState &self, const double longitude) {
        set_value(self, f_env_state_set_longitude, longitude);
    }

    static auto get_altitude(const EnvState &self) {
        return get_value(self, f_env_state_get_altitude);
    }

    static void set_altitude(const EnvState &self, const double altitude) {
        set_value(self, f_env_state_set_altitude, altitude);
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
        return get_value(self, f_env_state_sat_vapor_pressure);
    }

};
