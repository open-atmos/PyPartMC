/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "gimmicks.hpp"
#include "pmc_resource.hpp"

extern "C" void f_env_state_ctor(void *ptr) noexcept;
extern "C" void f_env_state_dtor(void *ptr) noexcept;
extern "C" void f_env_state_from_json(const void *ptr) noexcept;
extern "C" void f_env_state_set_temperature(const void *ptr, const double *temperature) noexcept;
extern "C" void f_env_state_get_temperature(const void *ptr, double *temperature) noexcept;
extern "C" void f_env_state_get_rel_humid(const void *ptr, double *rel_humid) noexcept;
extern "C" void f_env_state_set_height(const void *ptr, const double *height) noexcept;
extern "C" void f_env_state_get_height(const void *ptr, double *height) noexcept;
extern "C" void f_env_state_set_pressure(const void *ptr, const double *pressure) noexcept;
extern "C" void f_env_state_get_pressure(const void *ptr, double *pressure) noexcept;
extern "C" void f_env_state_get_elapsed_time(const void *ptr, double *elapsed_time) noexcept;
extern "C" void f_env_state_get_start_time(const void *ptr, double *start_time) noexcept;


struct EnvState {
    PMCResource ptr;

    EnvState(const nlohmann::json &json) :
        ptr(f_env_state_ctor, f_env_state_dtor)
    {
        GimmickGuard<InputGimmick> guard(json);
        f_env_state_from_json(this->ptr.f_arg());
    }

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
};
