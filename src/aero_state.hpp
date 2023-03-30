/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "aero_data.hpp"
#include "aero_dist.hpp"
#include "aero_particle.hpp"
#include "env_state.hpp"
#include "bin_grid.hpp"
#include "pybind11/stl.h"

extern "C" void f_aero_state_ctor(
    void *ptr
) noexcept;

extern "C" void f_aero_state_dtor(
    void *ptr
) noexcept;

extern "C" void f_aero_state_init(
    const void *ptr,
    const double *n_part,
    const void *aero_dataptr
) noexcept;

extern "C" void f_aero_state_len(
    const void *ptr, int *len
) noexcept;

extern "C" void f_aero_state_total_num_conc(
    const void *ptr,
    const void *aero_dataptr,
    double *total_num_conc
) noexcept;

extern "C" void f_aero_state_total_mass_conc(
    const void *ptr,
    const void *aero_dataptr,
    double *total_mass_conc
) noexcept;

extern "C" void f_aero_state_num_concs(
    const void *ptr,
    const void *aero_dataptr, 
    double *num_concs,
    const int *len
) noexcept;

extern "C" void f_aero_state_masses(
    const void *ptr,
    const void *aero_dataptr,
    double *masses,
    const int *n_parts
) noexcept;

extern "C" void f_aero_state_dry_diameters(
    const void *ptr,
    const void *aero_dataptr,
    double *dry_diameters,
    const int *n_parts
) noexcept;

extern "C" void f_aero_state_diameters(
    const void *ptr,
    const void *aero_dataptr,
    double *diameters,
    const int *n_parts
) noexcept;

extern "C" void f_aero_state_volumes(
    const void *ptr,
    const void *aero_dataptr,
    double *volumes,
    const int *n_parts
) noexcept;

extern "C" void f_aero_state_crit_rel_humids(
    const void *ptr,
    const void *aero_dataptr,
    const void *env_stateptr,
    double *crit_rel_humids,
    const int *n_parts
) noexcept;

extern "C" void f_aero_state_mixing_state_metrics(
    const void *aero_state, 
    const void *aero_data,
    double *d_alpha,
    double *d_gamma,
    double *chi
) noexcept;

extern "C" void f_aero_state_bin_average_comp(
    const void *ptr_c,
    const void *bin_grid_ptr, 
    const void *aero_data_ptr
) noexcept;

extern "C" void f_aero_state_copy(
    const void *ptr_c,
    const void *aero_dataptr
) noexcept;

extern "C" void f_aero_state_particle(
    const void *ptr_c,
    const void *ptr_particle_c,
    const int *index
) noexcept;

extern "C" void f_aero_state_rand_particle(
    const void *ptr_c,
    const void *ptr_particle_c
) noexcept;

extern "C" void f_aero_state_add_aero_dist_sample(
    const void *ptr_c, 
    const void *ptr_aero_data_c, 
    const void *ptr_aero_dist_c, 
    const double *sample_prop,
    const double *create_time,
    const bool *allow_doubling,
    const bool *allow_halving,
    int *n_part_add
) noexcept;

struct AeroState {
    PMCResource ptr;
    std::shared_ptr<AeroData> aero_data;

    AeroState(
        const double &n_part,
        std::shared_ptr<AeroData> aero_data
    ):
        ptr(f_aero_state_ctor, f_aero_state_dtor),
        aero_data(aero_data)
    {
        f_aero_state_init(
            ptr.f_arg(),
            &n_part,
            aero_data->ptr.f_arg()
        );
    }

    static std::size_t __len__(const AeroState &self) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        return len;
    }

    static auto total_num_conc(const AeroState &self) {
        double total_num_conc;
        f_aero_state_total_num_conc(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            &total_num_conc
        );
        return total_num_conc;
    }

    static auto total_mass_conc(const AeroState &self) {
        double total_mass_conc;
        f_aero_state_total_mass_conc(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            &total_mass_conc
        );
        return total_mass_conc;
    }

    static auto num_concs(const AeroState &self) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> num_concs(len);

        f_aero_state_num_concs(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            begin(num_concs),
            &len
        );

        return num_concs;
    }

    static auto masses(
        const AeroState &self
    ) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> masses(len);

        f_aero_state_masses(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            begin(masses),
            &len
        );

        return masses;
    }

    static auto dry_diameters(const AeroState &self) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> dry_diameters(len);

        f_aero_state_dry_diameters(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            begin(dry_diameters),
            &len
        );

        return dry_diameters;
    }

    static auto diameters(
       const AeroState &self
    ) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> diameters(len);

        f_aero_state_diameters(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            begin(diameters),
            &len
        );

        return diameters;
    }

    static auto volumes(
        const AeroState &self
    ) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> volumes(len);

        f_aero_state_volumes(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            begin(volumes),
            &len
        );

        return volumes;
    }

    static auto crit_rel_humids(
        const AeroState &self,
        const EnvState &env_state
    ) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> crit_rel_humids(len);

        f_aero_state_crit_rel_humids(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            env_state.ptr.f_arg(), 
            begin(crit_rel_humids),
            &len
        );

        return crit_rel_humids;
    }

    static auto mixing_state(
        const AeroState &self
    ) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        double chi;
        double d_alpha;
        double d_gamma;

        f_aero_state_mixing_state_metrics(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            &d_alpha,
            &d_gamma,
            &chi
        );

        return std::make_tuple(d_alpha, d_gamma, chi); 
    }

    static void bin_average_comp(
        AeroState &self,
        const BinGrid &bin_grid
    ) {
        f_aero_state_bin_average_comp(
            self.ptr.f_arg(),
            bin_grid.ptr.f_arg(),
            self.aero_data->ptr.f_arg()
        );
    }

    static AeroState* __deepcopy__(
        AeroState &self,
        py::dict &memo
    ) {
        double n_part = 1.0;
        AeroState *ptr = new AeroState(n_part, self.aero_data);
        f_aero_state_copy(
            self.ptr.f_arg(),
            ptr
        );
        return ptr;
    }

    static AeroParticle* get_particle(
        const AeroState &self,
        const int &idx
    ) {
        if (idx < 0 || idx >= (int)__len__(self))
            throw std::out_of_range("Index out of range");

        int len = AeroData::__len__(*self.aero_data);
        std::valarray<double> data(len);
        
        AeroParticle *ptr = new AeroParticle(self.aero_data, data);
        f_aero_state_particle(self.ptr.f_arg(), ptr, &idx);
        
        return ptr;
    } 

    static AeroParticle* get_random_particle(
        const AeroState &self
    ) {
        int len = AeroData::__len__(*self.aero_data);
        std::valarray<double> data(len);

        AeroParticle *ptr = new AeroParticle(self.aero_data, data);
        f_aero_state_rand_particle(self.ptr.f_arg(), ptr);

        return ptr;
    }

   static int dist_sample(
       const AeroState &self,
       const AeroDist &aero_dist,
       const double &sample_prop,
       const double &create_time,
       const bool &allow_doubling,
       const bool &allow_halving
   ) {
       int n_part_add = 0;

       f_aero_state_add_aero_dist_sample(
       self.ptr.f_arg(),
       self.aero_data->ptr.f_arg(),
       aero_dist.ptr.f_arg(),
       &sample_prop,
       &create_time,
       &allow_doubling,
       &allow_halving,
       &n_part_add
       );
       return n_part_add;

   }
};
