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
#include "tl/optional.hpp"

extern "C" void f_aero_state_ctor(
    void *ptr
) noexcept;

extern "C" void f_aero_state_dtor(
    void *ptr
) noexcept;

extern "C" void f_aero_state_init(
    const void *ptr,
    const void *aero_dataptr,
    const double *n_part,
    const char *weight_c
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
    const int *n_parts,
    const int *include_size,
    const int *exclude_size,
    void *include,
    void *exclude
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
    const int *n_parts,
    const int *include_size,
    const int *exclude_size,
    void *include,
    void *exclude
) noexcept;

extern "C" void f_aero_state_volumes(
    const void *ptr,
    const void *aero_dataptr,
    double *volumes,
    const int *n_parts,
    const int *include_size,
    const int *exclude_size,
    void *include,
    void *exclude
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
    double *chi,
    const int *include_size,
    const int *exclude_size,
    const int *group_size,
    void *include,
    void *exclude,
    void *group
) noexcept;

extern "C" void f_aero_state_bin_average_comp(
    const void *ptr_c,
    const void *bin_grid_ptr, 
    const void *aero_data_ptr
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


template <typename arr_t, typename arg_t>
auto pointer_vec_magic(arr_t &data_vec, const arg_t &arg) {
    std::vector<char*> pointer_vec(data_vec.size());
    if (arg.has_value())
        for (size_t i=0; i < arg.value().size(); ++i)
            strcpy(
                pointer_vec[i] = data_vec[i].data(),
                arg.value()[i].c_str()
            );
    return pointer_vec;
}


struct AeroState {
    PMCResource ptr;
    std::shared_ptr<AeroData> aero_data;

    AeroState(
        std::shared_ptr<AeroData> aero_data,
        const double &n_part,
        const bpstd::string_view &weight
    ):
        ptr(f_aero_state_ctor, f_aero_state_dtor),
        aero_data(aero_data)
    {
        static const std::map<bpstd::string_view, char> weight_c{
          //{"none", '-'},
          {"flat", 'f'},
          {"flat_source", 'F'},
          //{"power", 'p'},
          //{"power_source", 'P'},
          {"nummass", 'n'},
          {"nummass_source", 'N'},
        };

        if (weight_c.find(weight) == weight_c.end()) {
            std::ostringstream msg;
            msg << "unknown weighting scheme '" << weight << "', valid options are: ";
            auto index = 0;
            for (auto const& pair: weight_c)
                msg << (!index++ ? "" : ", ") << pair.first;
            throw std::runtime_error(msg.str());
        }

        f_aero_state_init(
            ptr.f_arg(),
            aero_data->ptr.f_arg(),
            &n_part,
            &weight_c.at(weight)
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
        const AeroState &self,
        const tl::optional<std::valarray<std::string>> &include,
        const tl::optional<std::valarray<std::string>> &exclude
    ) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> masses(len);

        const int include_size = (include.has_value()) ? include.value().size() : 0;
        const int exclude_size = (exclude.has_value()) ? exclude.value().size() : 0;

        std::vector<std::array<char, AERO_NAME_LEN>>
            include_arr(include_size),
            exclude_arr(exclude_size);

        f_aero_state_masses(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            begin(masses),
            &len,
            &include_size,
            &exclude_size,
            pointer_vec_magic(include_arr, include).data(),
            pointer_vec_magic(exclude_arr, exclude).data()
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
        const AeroState &self,
        const tl::optional<std::valarray<std::string>> &include,
        const tl::optional<std::valarray<std::string>> &exclude
    ) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> diameters(len);

        const int include_size = (include.has_value()) ? include.value().size() : 0;
        const int exclude_size = (exclude.has_value()) ? exclude.value().size() : 0;

        std::vector<std::array<char, AERO_NAME_LEN>>
            include_arr(include_size),
            exclude_arr(exclude_size);

        f_aero_state_diameters(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            begin(diameters),
            &len,
            &include_size,
            &exclude_size,
            pointer_vec_magic(include_arr, include).data(),
            pointer_vec_magic(exclude_arr, exclude).data()
        );

        return diameters;
    }

    static auto volumes(
        const AeroState &self,
        const tl::optional<std::valarray<std::string>> &include,
        const tl::optional<std::valarray<std::string>> &exclude
    ) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        std::valarray<double> volumes(len);

        const int include_size = (include.has_value()) ? include.value().size() : 0;
        const int exclude_size = (exclude.has_value()) ? exclude.value().size() : 0;

        std::vector<std::array<char, AERO_NAME_LEN>>
            include_arr(include_size),
            exclude_arr(exclude_size);

        f_aero_state_volumes(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            begin(volumes),
            &len,
            &include_size,
            &exclude_size,
            pointer_vec_magic(include_arr, include).data(),
            pointer_vec_magic(exclude_arr, exclude).data()
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
        const AeroState &self,
        const tl::optional<std::valarray<std::string>> &include,
        const tl::optional<std::valarray<std::string>> &exclude,
        const tl::optional<std::valarray<std::string>> &group
    ) {
        int len;
        f_aero_state_len(
            self.ptr.f_arg(),
            &len
        );
        double chi;
        double d_alpha;
        double d_gamma;

        const int include_size = (include.has_value()) ? include.value().size() : 0;
        const int exclude_size = (exclude.has_value()) ? exclude.value().size() : 0;
        const int group_size = (group.has_value()) ? group.value().size() : 0;

        std::vector<std::array<char, AERO_NAME_LEN>>
            include_arr(include_size),
            exclude_arr(exclude_size),
            group_arr(group_size);

        f_aero_state_mixing_state_metrics(
            self.ptr.f_arg(),
            self.aero_data->ptr.f_arg(),
            &d_alpha,
            &d_gamma,
            &chi,
            &include_size,
            &exclude_size,
            &group_size,
            pointer_vec_magic(include_arr, include).data(),
            pointer_vec_magic(exclude_arr, exclude).data(),
            pointer_vec_magic(group_arr, group).data()
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
