/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "aero_data.hpp"
#include "env_state.hpp"
#include "pybind11/stl.h"
#include <complex>

extern "C" void f_aero_particle_ctor(void *ptr) noexcept;
extern "C" void f_aero_particle_dtor(void *ptr) noexcept;
extern "C" void f_aero_particle_init(const void *ptr, const void *, const void *arr_data, const int *arr_size) noexcept;
extern "C" void f_aero_particle_volumes(const void *ptr, void *arr_data, const int *arr_size) noexcept;
extern "C" void f_aero_particle_volume(const void *ptr, double *vol) noexcept;
extern "C" void f_aero_particle_species_volume(const void *ptr, const int *i_spec, double *vol) noexcept;
extern "C" void f_aero_particle_dry_volume(const void *aero_particle_ptr, const void *aero_data_ptr, double *vol) noexcept;
extern "C" void f_aero_particle_radius(const void *aero_particle_ptr, const void *aero_data_ptr, double *radius) noexcept;
extern "C" void f_aero_particle_dry_radius(const void *aero_particle_ptr, const void *aero_data_ptr, double *radius) noexcept;
extern "C" void f_aero_particle_diameter(const void *aero_particle_ptr, const void *aero_data_ptr, double *diameter) noexcept;
extern "C" void f_aero_particle_dry_diameter(const void *aero_particle_ptr, const void * aero_data_ptr, double *diameter) noexcept;
extern "C" void f_aero_particle_mass(const void *aero_particle_ptr, const void *aero_data_ptr, double *mass) noexcept;
extern "C" void f_aero_particle_species_mass(const void *aero_particle_ptr, const int *i_spec, const void *aero_data_ptr, double *mass) noexcept;
extern "C" void f_aero_particle_species_masses(const void *aero_particle_ptr, const void *aero_data_ptr, const int *size_masses, void *masses) noexcept;
extern "C" void f_aero_particle_solute_kappa(const void *aero_particle_ptr, const void *aero_data_ptr, void *kappa) noexcept;
extern "C" void f_aero_particle_moles(const void *aero_particle_ptr, const void *aero_data_ptr, void *moles) noexcept;
extern "C" void f_aero_particle_mobility_diameter(const void *aero_particle_ptr, const void *aero_data_ptr, const void *env_state_ptr, void *mobility_diameter) noexcept;
extern "C" void f_aero_particle_density(const void *aero_particle_ptr, const void *aero_data_ptr, void *density) noexcept;
extern "C" void f_aero_particle_approx_crit_rel_humid(const void *aero_particle_ptr, const void *aero_data_ptr, const void *env_state_ptr, void *approx_crit_rel_humid) noexcept;
extern "C" void f_aero_particle_crit_rel_humid(const void *aero_particle_ptr, const void *aero_data_ptr, const void *env_state_ptr, void *crit_rel_humid) noexcept;
extern "C" void f_aero_particle_crit_diameter(const void *aero_particle_ptr, const void *aero_data_ptr, const void *env_state, void *crit_diameter) noexcept;
extern "C" void f_aero_particle_coagulate(const void *aero_particle_1_ptr, const void *aero_particle_2_ptr, void *new_particle_ptr) noexcept;
extern "C" void f_aero_particle_zero(void *aero_particle_ptr, const void *aero_data_ptr) noexcept;
extern "C" void f_aero_particle_set_vols(void *aero_particle_ptr, const int *vol_size, const void *volumes) noexcept;
extern "C" void f_aero_particle_absorb_cross_sect(const void *aero_particle_ptr, double *val) noexcept;
extern "C" void f_aero_particle_scatter_cross_sect(const void *aero_particle_ptr, double *val) noexcept;
extern "C" void f_aero_particle_asymmetry(const void *aero_particle_ptr, double *val) noexcept;
extern "C" void f_aero_particle_greatest_create_time(const void *aero_particle_ptr, double *val) noexcept;
extern "C" void f_aero_particle_least_create_time(const void *aero_particle_ptr, double *val) noexcept;
extern "C" void f_aero_particle_n_orig_part(const void *aero_particle_ptr, void *arr_data, const int *arr_size) noexcept;
extern "C" void f_aero_particle_id(const void *aero_particle_ptr, int *val) noexcept;
extern "C" void f_aero_particle_refract_shell(const void *aero_particle_ptr, std::complex<double> *val) noexcept;
extern "C" void f_aero_particle_refract_core(const void *aero_particle_ptr, std::complex<double> *val) noexcept;

namespace py = pybind11;
struct AeroParticle {
    PMCResource ptr;
    std::shared_ptr<AeroData> aero_data;

    AeroParticle(
        std::shared_ptr<AeroData> aero_data,
        const std::valarray<double>&data
    ) :
        ptr(f_aero_particle_ctor, f_aero_particle_dtor),
        aero_data(aero_data)
    {
        int len = data.size();
        f_aero_particle_init(ptr.f_arg(), aero_data->ptr.f_arg(), begin(data), &len);
        if (size_t(len) != AeroData::__len__(*aero_data))
            throw std::runtime_error("AeroData size mistmatch");
    }

    static auto volumes(const AeroParticle &self)
    {
        int len = AeroData::__len__(*self.aero_data);
        std::valarray<double> data(len);
        f_aero_particle_volumes(
            self.ptr.f_arg(),
            begin(data),
            &len
        );
        return data;
    }

    static auto volume(const AeroParticle &self) {
        double vol;
        f_aero_particle_volume(
            self.ptr.f_arg(),
            &vol
        );
        return vol;
    }

    static auto species_volume(const AeroParticle &self, const int &i_spec) {
        double vol;
        f_aero_particle_species_volume(
            self.ptr.f_arg(),
            &i_spec,
            &vol
        );
        return vol;
    }

    static auto species_volume_by_name(const AeroParticle &self, const std::string &name) {
        double vol;
        const int i_spec = AeroData::spec_by_name(*self.aero_data, name);

        f_aero_particle_species_volume(
            self.ptr.f_arg(),
            &i_spec,
            &vol
        );
        return vol;
    }

    static auto dry_volume(const AeroParticle &self) {
        double vol;
        f_aero_particle_dry_volume(
            self.ptr.f_arg(),
            self.aero_data.get(),
            &vol
        );
        return vol;
    }

    static auto radius(const AeroParticle &self) {
        double radius;
        f_aero_particle_radius(
            self.ptr.f_arg(),
            self.aero_data.get(),
            &radius
        );
        return radius;
    }

    static auto dry_radius(const AeroParticle &self) {
        double radius;
        f_aero_particle_dry_radius(
            self.ptr.f_arg(),
            self.aero_data.get(),
            &radius
        );
        return radius;
    }

    static auto diameter(const AeroParticle &self) {
        double diameter;
        f_aero_particle_diameter(
            self.ptr.f_arg(),
            self.aero_data.get(),
            &diameter
        );
        return diameter;
    }

    static auto dry_diameter(const AeroParticle &self) {
        double diameter;
        f_aero_particle_dry_diameter(
            self.ptr.f_arg(),
            self.aero_data.get(),
            &diameter
        );
        return diameter;
    }

    static auto mass(const AeroParticle &self) {
        double mass;
        f_aero_particle_mass(
            self.ptr.f_arg(),
            self.aero_data.get(),
            &mass
        );
        return mass;
    }

    static auto species_mass(const AeroParticle &self, const int &i_spec) {
        double mass;
        f_aero_particle_species_mass(
            self.ptr.f_arg(),
            &i_spec,
            self.aero_data.get(),
            &mass
        );
        return mass;
    }

    static auto species_mass_by_name(const AeroParticle &self, const std::string &name) {
        double mass;
        const int i_spec = AeroData::spec_by_name(*self.aero_data, name);
        f_aero_particle_species_mass(
            self.ptr.f_arg(),
            &i_spec,
            self.aero_data.get(),
            &mass
        );
        return mass;
    }

    static auto species_masses(const AeroParticle &self) {
        int len = AeroData::__len__(*self.aero_data);
        std::valarray<double> masses(len);
        f_aero_particle_species_masses(
            self.ptr.f_arg(),
            self.aero_data.get(),
            &len,
            begin(masses)
        );
        return masses;
    }

    static auto solute_kappa(const AeroParticle &self) {
        double kappa;
        f_aero_particle_solute_kappa(
            self.ptr.f_arg(),
            self.aero_data.get(),
            &kappa
        );
        return kappa;
    }

    static auto moles(const AeroParticle &self) {
        double moles;
        f_aero_particle_moles(
            self.ptr.f_arg(),
            self.aero_data.get(),
            &moles
        );
        return moles;
    }

    static auto mobility_diameter(const AeroParticle &self, const EnvState &env_state) {
        double mobility_diameter;
        f_aero_particle_mobility_diameter(
            self.ptr.f_arg(),
            self.aero_data.get(),
            env_state.ptr.f_arg(),
            &mobility_diameter
        );
        return mobility_diameter;
    }

    static auto density(const AeroParticle &self) {
        double density;
        f_aero_particle_density(
            self.ptr.f_arg(),
            self.aero_data.get(),
            &density
        );
        return density;
    }

    static auto approx_crit_rel_humid(const AeroParticle &self, const EnvState &env_state) {
        double approx_crit_rel_humid;
        f_aero_particle_approx_crit_rel_humid(
            self.ptr.f_arg(),
            self.aero_data.get(),
            env_state.ptr.f_arg(),
            &approx_crit_rel_humid
        );
        return approx_crit_rel_humid;
    }

    static auto crit_rel_humid(const AeroParticle &self, const EnvState &env_state) {
        double crit_rel_humid;
        f_aero_particle_crit_rel_humid(
            self.ptr.f_arg(),
            self.aero_data.get(),
            env_state.ptr.f_arg(),
            &crit_rel_humid
        );
        return crit_rel_humid;
    }

    static auto crit_diameter(const AeroParticle &self, const EnvState &env_state) {
        double crit_diameter;
        f_aero_particle_crit_diameter(
            self.ptr.f_arg(),
            self.aero_data.get(),
            env_state.ptr.f_arg(),
            &crit_diameter
        );
        return crit_diameter;
    }

    static auto coagulate(const AeroParticle &self, const AeroParticle &two) {
        int len = AeroData::__len__(*self.aero_data);
        std::valarray<double> data(len);
        AeroParticle* new_ptr = new AeroParticle(self.aero_data, data);
        f_aero_particle_coagulate(
            self.ptr.f_arg(),
            two.ptr.f_arg(),
            new_ptr
        );
        return new_ptr;
    }

    static void zero(AeroParticle &self) {
        f_aero_particle_zero(
            self.ptr.f_arg_non_const(),
            self.aero_data.get()
        );
    }

    static void set_vols(AeroParticle &self, const std::valarray<double>&volumes) {
        int len = AeroData::__len__(*self.aero_data.get());
        if (volumes.size() != size_t(len))
            throw std::runtime_error("AeroData size mistmatch");
        f_aero_particle_set_vols(
            self.ptr.f_arg_non_const(),
            &len,
            begin(volumes)
        );
    }

    static auto scatter_cross_sect(const AeroParticle &self) {
        double val;
        f_aero_particle_scatter_cross_sect(
            self.ptr.f_arg(),
            &val
        );
        return val;
    }

    static auto absorb_cross_sect(const AeroParticle &self) {
        double val;
        f_aero_particle_absorb_cross_sect(
            self.ptr.f_arg(),
            &val
        );
        return val;
    }

    static auto asymmetry(const AeroParticle &self) {
        double val;
        f_aero_particle_asymmetry(
            self.ptr.f_arg(),
            &val
        );
        return val;
    }

    static auto n_orig_part(const AeroParticle &self) {
        int len = AeroData::n_source(*self.aero_data);
        std::valarray<int> data(len);

        f_aero_particle_n_orig_part(
            self.ptr.f_arg(),
            begin(data),
            &len
        );
        return data;
    }

    static auto least_create_time(const AeroParticle &self) {
        double val;
        f_aero_particle_least_create_time(
            self.ptr.f_arg(),
            &val
        );
        return val;
    }

    static auto greatest_create_time(const AeroParticle &self) {
        double val;
        f_aero_particle_greatest_create_time(
            self.ptr.f_arg(),
            &val
        );
        return val;
    }

    static auto id(const AeroParticle &self) {
        int val;
        f_aero_particle_id(
            self.ptr.f_arg(),
            &val
        );
        return val;
    }

    static auto refract_shell(const AeroParticle &self) {
        std::complex<double> refract_shell;
        f_aero_particle_refract_shell(
            self.ptr.f_arg(),
            &refract_shell
        );
        return refract_shell;
    }

    static auto refract_core(const AeroParticle &self) {
        std::complex<double> refract_core;
        f_aero_particle_refract_core(
            self.ptr.f_arg(),
            &refract_core
        );
        return refract_core;
    }
};
