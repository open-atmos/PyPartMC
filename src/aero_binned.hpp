/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "aero_data.hpp"
#include "pybind11/stl.h"

extern "C" void f_bin_grid_ctor(void *ptr) noexcept;

extern "C" void f_bin_grid_dtor(void *ptr) noexcept;

struct AeroBinned {
    PMCResource ptr;
    std::shared_ptr<AeroData> aero_data;
    AeroBinned(std::shared_ptr<AeroData> aero_data) : 
        ptr(f_bin_grid_ctor, f_bin_grid_dtor),
        aero_data(aero_data)
    {
    }

};

