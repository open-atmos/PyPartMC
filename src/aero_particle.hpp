/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "aero_data.hpp"
#include "pybind11/stl.h"

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

};

