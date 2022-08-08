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

namespace py = pybind11;
struct AeroParticle {
    PMCResource ptr;
    const AeroData &aero_data;

    AeroParticle( const AeroData &aero_data, const std::valarray<double>&data) :
        ptr(f_aero_particle_ctor, f_aero_particle_dtor),
        aero_data(aero_data)
    {
        int len = data.size();
        f_aero_particle_init(ptr.f_arg(), aero_data.ptr.f_arg(), begin(data), &len);
        if (size_t(len)!=AeroData::__len__(aero_data))
            throw std::runtime_error("AeroData size mistmatch");
    }

    static std::valarray<double> volumes(const AeroParticle &self)
    {
        int len = AeroData::__len__(self.aero_data);
        std::valarray<double> data(len);
        f_aero_particle_volumes(&self.ptr, begin(data), &len);
        return data;
    }

    static double particle_volume(const AeroParticle &self) {
        double vol;
        f_aero_particle_volume(&self.ptr, &vol);
        return vol;
    }

    static double species_volume(const AeroParticle &self, const int &i_spec) {
        double vol;
        f_aero_particle_species_volume(&self.ptr, &i_spec, &vol);
        return vol;
    }
};

