/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "aero_mode.hpp"

extern "C" void f_aero_dist_ctor(
    void *ptr
) noexcept;

extern "C" void f_aero_dist_dtor(
    void *ptr
) noexcept;

extern "C" void f_aero_dist_n_mode(
    const void *ptr,
    int *n_mode
) noexcept;

extern "C" void f_aero_dist_from_json(
    void *ptr,
    void *aero_data_ptr
) noexcept;

extern "C" void f_aero_dist_total_num_conc(
    const void *ptr,
    double *total_num_conc
) noexcept;

extern "C" void f_aero_dist_mode(
    const void *ptr,
    void *ptr_c,
    const int *index
) noexcept;

struct AeroDist {
    PMCResource ptr;
    std::shared_ptr<AeroData> aero_data;

    AeroDist(
        std::shared_ptr<AeroData> aero_data,
        const nlohmann::json &json
    ):
        ptr(f_aero_dist_ctor, f_aero_dist_dtor),
        aero_data(aero_data)
    {
        if (!InputGimmick::unique_keys(json))
            throw std::runtime_error("Mode names must be unique");
        
        GimmickGuard<InputGimmick> guard(json, "", "mode_name", 1);
        f_aero_dist_from_json(ptr.f_arg_non_const(), aero_data->ptr.f_arg_non_const());
    }

    AeroDist() :
        ptr(f_aero_dist_ctor, f_aero_dist_dtor)
    {}

    static auto get_n_mode(const AeroDist &self) {
        int n_mode;
        f_aero_dist_n_mode(self.ptr.f_arg(), &n_mode);
        return n_mode;
    }

    static auto get_total_num_conc(const AeroDist &self) {
        double total_num_conc;
        f_aero_dist_total_num_conc(self.ptr.f_arg(), &total_num_conc);
        return total_num_conc;
    }

    static AeroMode* get_mode(const AeroDist &self, const int &idx) {
        if (idx < 0 || idx >= AeroDist::get_n_mode(self))
            throw std::out_of_range("Index out of range");

        AeroMode *ptr = new AeroMode();
        f_aero_dist_mode(self.ptr.f_arg(), ptr, &idx);

        return ptr;
    }
};
