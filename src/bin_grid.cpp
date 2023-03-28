/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "bin_grid.hpp"

std::valarray<double> histogram_1d(
    const BinGrid &bin_grid,
    std::valarray<double> values,
    std::valarray<double> weights
) {
    int len;
    f_bin_grid_size(
        bin_grid.ptr.f_arg(),
        &len
    );
    int data_size = values.size();
    std::valarray<double> data(len);
    f_bin_grid_histogram_1d(
        bin_grid.ptr.f_arg(),
        begin(values),
        begin(weights),
        &data_size,
        begin(data),
        &len
    );

    return data;
}

std::vector<std::vector<double>> histogram_2d(
    const BinGrid &x_bin_grid,
    std::valarray<double> x_values,
    const BinGrid &y_bin_grid,
    std::valarray<double> y_values,
    std::valarray<double> weights
) {
    int x_len;
    f_bin_grid_size(x_bin_grid.ptr.f_arg(), &x_len);

    int y_len;
    f_bin_grid_size(y_bin_grid.ptr.f_arg(), &y_len);

    const int data_size = x_values.size();

    std::vector<std::vector<double>> data(
        x_len,
        std::vector<double>(y_len, 0)
    );
    std::valarray<double> data_flat(x_len * y_len);
    f_bin_grid_histogram_2d(
        x_bin_grid.ptr.f_arg(),
        begin(x_values),
        y_bin_grid.ptr.f_arg(),
        begin(y_values),
        begin(weights),
        &data_size, 
        begin(data_flat),
        &x_len,
        &y_len
    );

    for(int i = 0; i < x_len; i++) {
        for(int j = 0; j < y_len; j++) {
            data[i][j] = data_flat[i*y_len + j];
        }
    }

    return data;
}
