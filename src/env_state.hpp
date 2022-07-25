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

struct EnvState {
    PMCResource ptr;

    EnvState(const nlohmann::json &json) :
        ptr(f_env_state_ctor, f_env_state_dtor)
    {
        gimmick_ptr() = std::make_unique<InputGimmick>(json); // TODO #117: guard
        f_env_state_from_json(this->ptr.f_arg());
        gimmick_ptr().reset();
    }

    static void set_temperature(const EnvState &self, double &temperature){
        f_env_state_set_temperature(&self.ptr, &temperature);
    }

    static double temp(const EnvState &self){
        double temperature;

        f_env_state_get_temperature(&self.ptr, &temperature);
        return temperature;
    }

    static double rh(const EnvState &self){
        double rel_humid;

        f_env_state_get_rel_humid(&self.ptr, &rel_humid);
        return rel_humid;
    }
    
    static void set_height(const EnvState &self, const double height) {
        f_env_state_set_height(&self.ptr, &height);
    }

    static double get_height(const EnvState &self) {
        double height;

        f_env_state_get_height(&self.ptr, &height);
        return height;
    }

    static void set_pressure(const EnvState &self, const double pressure) {
        f_env_state_set_pressure(&self.ptr, &pressure);
    }

    static double get_pressure(const EnvState &self) {
        double pressure;

        f_env_state_get_pressure(&self.ptr, &pressure);
        return pressure;
    }
};
