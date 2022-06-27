/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "aero_data.hpp"
#include "env_state.hpp"
#include "bin_grid.hpp"
#include "pybind11/stl.h"

extern "C" void f_aero_state_ctor(void *ptr) noexcept;
extern "C" void f_aero_state_dtor(void *ptr) noexcept;
extern "C" void f_aero_state_init(
    const void *ptr,
    const double *n_part,
    const void *aero_dataptr
) noexcept;
extern "C" void f_aero_state_len(const void *ptr, int *len) noexcept;
extern "C" void f_aero_state_total_num_conc(const void *ptr, const void *aero_dataptr,
    double *total_num_conc) noexcept;
extern "C" void f_aero_state_total_mass_conc(const void *ptr, const void *aero_dataptr,
    double *total_mass_conc) noexcept;
extern "C" void f_aero_state_num_concs(const void *ptr, const void *aero_dataptr, 
    double *num_concs, const int *len) noexcept;
extern "C" void f_aero_state_masses(const void *ptr, const void *aero_dataptr,
    double *masses, const int *n_parts) noexcept;
extern "C" void f_aero_state_dry_diameters(const void *ptr, const void *aero_dataptr,
    double *dry_diameters, const int *n_parts) noexcept;
extern "C" void f_aero_state_diameters(const void *ptr, const void *aero_dataptr,
    double *diameters, const int *n_parts) noexcept;
extern "C" void f_aero_state_volumes(const void *ptr, const void *aero_dataptr,
    double *volumes, const int *n_parts) noexcept;
extern "C" void f_aero_state_crit_rel_humids(const void *ptr, const void *aero_dataptr,
       const void *env_stateptr, double *crit_rel_humids, const int *n_parts) noexcept;
extern "C" void f_aero_state_mixing_state_metrics(const void *aero_state, 
       const void *aero_data, double *d_alpha, double *d_gamma, double *chi) noexcept;
extern "C" void f_aero_state_bin_average_comp(const void *ptr_c, const void *bin_grid_ptr, 
       const void *aero_data_ptr) noexcept;

struct AeroState {
    PMCResource ptr;

    AeroState(const double &n_part, const AeroData &aero_data) :
        ptr(f_aero_state_ctor, f_aero_state_dtor)
    {
        f_aero_state_init(ptr.f_arg(), &n_part, aero_data.ptr.f_arg());
    }

    static std::size_t __len__(const AeroState &self) {
        int len;
        f_aero_state_len(&self.ptr, &len);
        return len;
    }

    static double total_num_conc(const AeroState &self, const AeroData &aero_data){
        double total_num_conc;
        f_aero_state_total_num_conc(&self.ptr, &aero_data.ptr, &total_num_conc);
        return total_num_conc;
    }

    static double total_mass_conc(const AeroState &self, const AeroData &aero_data){
        double total_mass_conc;
        f_aero_state_total_mass_conc(&self.ptr, &aero_data.ptr, &total_mass_conc);
        return total_mass_conc;
    }

    static std::valarray<double> num_concs(const AeroState &self, const AeroData &aero_data){
        int len;
        f_aero_state_len(&self.ptr, &len);
        std::valarray<double> num_concs(len);

        f_aero_state_num_concs(&self.ptr, &aero_data.ptr, begin(num_concs), &len);

        return num_concs;
    }

    static std::valarray<double> masses(const AeroState &self, const AeroData &aero_data){
        int len;
        f_aero_state_len(&self.ptr, &len);
        std::valarray<double> masses(len);

        f_aero_state_masses(&self.ptr, &aero_data.ptr, begin(masses), &len);

        return masses;
    }

   static std::valarray<double> dry_diameters(const AeroState &self, const AeroData &aero_data){
        int len;
        f_aero_state_len(&self.ptr, &len);
        std::valarray<double> dry_diameters(len);

        f_aero_state_dry_diameters(&self.ptr, &aero_data.ptr, begin(dry_diameters), &len);

        return dry_diameters;
    }

   static std::valarray<double> diameters(const AeroState &self, const AeroData &aero_data){
        int len;
        f_aero_state_len(&self.ptr, &len);
        std::valarray<double> diameters(len);

        f_aero_state_diameters(&self.ptr, &aero_data.ptr, begin(diameters), &len);

        return diameters;
    }

    static std::valarray<double> volumes(const AeroState &self, const AeroData &aero_data){
        int len;
        f_aero_state_len(&self.ptr, &len);
        std::valarray<double> volumes(len);

        f_aero_state_volumes(&self.ptr, &aero_data.ptr, begin(volumes), &len);

        return volumes;
    }

    static std::valarray<double> crit_rel_humids(const AeroState &self, const AeroData &aero_data,
        const EnvState &env_state){
        int len;
        f_aero_state_len(&self.ptr, &len);
        std::valarray<double> crit_rel_humids(len);

        f_aero_state_crit_rel_humids(&self.ptr, &aero_data.ptr, &env_state.ptr, 
             begin(crit_rel_humids), &len);

        return crit_rel_humids;
    }

    static std::tuple<double, double, double> mixing_state(const AeroState &self, 
        const AeroData &aero_data){

        int len;
        f_aero_state_len(&self.ptr, &len);
        double chi;
        double d_alpha;
        double d_gamma;

        f_aero_state_mixing_state_metrics(&self.ptr, &aero_data.ptr,
             &d_alpha, &d_gamma, &chi);

        return std::make_tuple(d_alpha, d_gamma, chi); 
    }

    static void bin_average_comp(AeroState &self, const BinGrid &bin_grid, 
        const AeroData &aero_data){

        f_aero_state_bin_average_comp(&self.ptr, &bin_grid.ptr, &aero_data.ptr);
    }
};
