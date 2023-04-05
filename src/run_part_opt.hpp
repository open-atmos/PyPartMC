/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "gimmicks.hpp"
#include "pybind11_json/pybind11_json.hpp"

extern "C" void f_run_part_opt_ctor(void *ptr) noexcept;
extern "C" void f_run_part_opt_dtor(void *ptr) noexcept;
extern "C" void f_run_part_opt_from_json(const void *ptr) noexcept;
extern "C" void f_run_part_opt_t_max(const void *ptr, double *t_max) noexcept;
extern "C" void f_run_part_opt_del_t(const void *ptr, double *del_t) noexcept;

struct RunPartOpt {
    PMCResource ptr;

    RunPartOpt(const nlohmann::json &json) :
        ptr(f_run_part_opt_ctor, f_run_part_opt_dtor)
    {
        gimmick_ptr() = std::make_unique<InputGimmick>(json); // TODO #117: guard
        f_run_part_opt_from_json(this->ptr.f_arg());
        gimmick_ptr().reset();
    }

    static auto t_max(const RunPartOpt &self){
        double t_max;

        f_run_part_opt_t_max(self.ptr.f_arg(), &t_max);

        return t_max;
    }

    static auto del_t(const RunPartOpt &self){
        double del_t;

        f_run_part_opt_del_t(self.ptr.f_arg(), &del_t);

        return del_t;
    }
};

