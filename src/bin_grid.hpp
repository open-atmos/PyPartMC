/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"

extern "C" void f_bin_grid_ctor(void *ptr) noexcept;
extern "C" void f_bin_grid_dtor(void *ptr) noexcept;
extern "C" void f_bin_grid_init(
    const void *ptr,
    const int *n_bin,
    const int *type,
    const double *min,
    const double *max
) noexcept;
extern "C" void f_bin_grid_size(const void *ptr, int *val) noexcept;
extern "C" void f_bin_grid_edges(const void *ptr, void *arr) noexcept;
extern "C" void f_bin_grid_centers(const void *ptr, void *arr) noexcept;

struct BinGrid {
    PMCResource ptr;

    BinGrid(const int &n_bin, const std::string &grid_type, const double &min, const double &max) :
        ptr(f_bin_grid_ctor, f_bin_grid_dtor)
    {
        int type;
        type = 0;
        if (grid_type == "log") type = 1;
        if (grid_type == "linear") type = 2;
        if (type == 0) throw std::invalid_argument( "Invalid grid spacing" );
        f_bin_grid_init(ptr.f_arg(), &n_bin, &type, &min, &max);
    }

    static std::size_t __len__(const BinGrid &self) {
        int len;
        f_bin_grid_size(&self.ptr, &len);
        return len;
    }

    static py::array_t<double> edges(const BinGrid &self) {

        int len;
        f_bin_grid_size(&self.ptr, &len);
        
        py::array_t<double> result = py::array_t<double>(len+1);
        auto buf = result.request();
        double *buf_ptr = (double *) buf.ptr;

        f_bin_grid_edges(&self.ptr, &buf_ptr);

        return result;
    }

    static py::array_t<double> centers(const BinGrid &self) {

        int len;
        f_bin_grid_size(&self.ptr, &len);

        py::array_t<double> result = py::array_t<double>(len);
        auto buf = result.request();
        double *buf_ptr = (double *) buf.ptr;

        f_bin_grid_centers(&self.ptr, &buf_ptr);

        return result;
    }

};
