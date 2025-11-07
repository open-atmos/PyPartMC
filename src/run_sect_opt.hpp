/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2025 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "json_resource.hpp"
#include "env_state.hpp"

extern "C" void f_run_sect_opt_ctor(void *ptr) noexcept;
extern "C" void f_run_sect_opt_dtor(void *ptr) noexcept;
extern "C" void f_run_sect_opt_from_json(const void *ptr, const void *env_state_ptr) noexcept;
extern "C" void f_run_sect_opt_t_max(const void *ptr, double *t_max) noexcept;
extern "C" void f_run_sect_opt_del_t(const void *ptr, double *del_t) noexcept;

struct RunSectOpt {
    PMCResource ptr;

    RunSectOpt(const nlohmann::ordered_json &json, EnvState &env_state) :
        ptr(f_run_sect_opt_ctor, f_run_sect_opt_dtor)
    {
        nlohmann::ordered_json json_copy(json);

        for (auto key : std::set<std::string>({
            "t_output", "t_progress"
        }))
            if (json_copy.find(key) == json_copy.end())
                json_copy[key] = 0;

        JSONResourceGuard<InputJSONResource> guard(json_copy);
        f_run_sect_opt_from_json(this->ptr.f_arg(), env_state.ptr.f_arg());
        guard.check_parameters();
    }

    static auto t_max(const RunSectOpt &self){
        double t_max;

        f_run_sect_opt_t_max(self.ptr.f_arg(), &t_max);

        return t_max;
    }

    static auto del_t(const RunSectOpt &self){
        double del_t;

        f_run_sect_opt_del_t(self.ptr.f_arg(), &del_t);

        return del_t;
    }
};

