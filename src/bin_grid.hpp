/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include "pmc_resource.hpp"
#include "pybind11/stl.h"

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
extern "C" void f_bin_grid_edges(const void *ptr, void *arr_data, const int *arr_size) noexcept;
extern "C" void f_bin_grid_centers(const void *ptr, void *arr_data, const int *arr_size) noexcept;
extern "C" void f_bin_grid_histogram_1d(const void *x_bin_grid_ptr_c, const void *x_data,
    const void *weight_data, const int *arr_size, void *output_data, const int *grid_size) noexcept;
extern "C" void f_bin_grid_histogram_2d(const void *x_bin_grid_ptr_c, const void *x_data,
    const void *y_bin_grid_ptr_c, const void *y_data,
    const void *weight_data, const int *arr_size, void *output_data, const int *x_grid_size,
    const int *y_grid_size) noexcept;

struct BinGrid {
    PMCResource ptr;

    BinGrid(const int &n_bin, const std::string &grid_type, const double &min, const double &max) :
        ptr(f_bin_grid_ctor, f_bin_grid_dtor)
    {
        int type;
        type = 0;
        if (grid_type == "log") type = 1;
        if (grid_type == "linear") type = 2;
        if (type == 0) throw std::invalid_argument( "Invalid grid spacing." );
        f_bin_grid_init(ptr.f_arg(), &n_bin, &type, &min, &max);
    }

    static std::size_t __len__(const BinGrid &self) {
        int len;
        f_bin_grid_size(&self.ptr, &len);
        return len;
    }

    static std::valarray<double> edges(const BinGrid &self)
    {
        int len;
        f_bin_grid_size(&self.ptr, &len);
        len++;
        std::valarray<double> data(len);
        f_bin_grid_edges(&self.ptr, begin(data), &len);
        return data;
    }

    static std::valarray<double> centers(const BinGrid &self) 
    {
        int len;
        f_bin_grid_size(&self.ptr, &len);
        std::valarray<double> data(len);
        f_bin_grid_centers(&self.ptr, begin(data), &len);
        return data;
    }
};

static std::valarray<double> histogram_1d(
    const BinGrid &bin_grid,
    std::valarray<double> values,
    std::valarray<double> weights){

        int len;
        f_bin_grid_size(&bin_grid.ptr, &len);
        int data_size = values.size();
        std::valarray<double> data(len);
        f_bin_grid_histogram_1d(&bin_grid.ptr, begin(values), begin(weights),
            &data_size, begin(data), &len);

        return data;
}

static std::vector<std::vector<double>> histogram_2d(
    const BinGrid &x_bin_grid,
    std::valarray<double> x_values,
    const BinGrid &y_bin_grid,
    std::valarray<double> y_values,
    std::valarray<double> weights){

        int x_len;
        f_bin_grid_size(&x_bin_grid.ptr, &x_len);
        int y_len;
        f_bin_grid_size(&y_bin_grid.ptr, &y_len);
        int data_size = x_values.size();

        std::vector<std::vector<double>> data( x_len , std::vector<double>(y_len,0));
        std::valarray<double> data_flat(x_len*y_len);
        f_bin_grid_histogram_2d(&x_bin_grid.ptr, begin(x_values),
            &y_bin_grid.ptr, begin(y_values), begin(weights),
            &data_size, begin(data_flat), &x_len, &y_len);

        for(int i = 0; i < x_len; i++)
        {
            for(int j = 0; j < y_len; j++)
            {
            data[i][j] = data_flat[i*y_len + j];
            }
        }

        return data;
}
