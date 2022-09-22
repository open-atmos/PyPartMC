####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import numpy as np

import PyPartMC as ppmc


class TestBinGrid:
    @staticmethod
    def test_ctor():
        # arrange
        pass

        # act
        sut = ppmc.BinGrid(123, "log", 1, 100)

        # assert
        assert sut is not None

    @staticmethod
    def test_len():
        # arrange
        grid_size = 666
        sut = ppmc.BinGrid(grid_size, "log", 1, 100)

        # act
        size = len(sut)

        # assert
        assert size == grid_size

    @staticmethod
    def test_bin_edges_len():
        # arrange
        grid_size = 100
        sut = ppmc.BinGrid(grid_size, "log", 1, 100)

        # act
        edges = sut.edges

        # assert
        assert grid_size + 1 == len(edges)

    @staticmethod
    def test_bin_edges_values():
        # arrange
        n_bins = 10
        left_edge = 1
        right_edge = 100
        sut = ppmc.BinGrid(n_bins, "log", left_edge, right_edge)

        # act
        edges = sut.edges

        # assert
        np.testing.assert_array_almost_equal(
            np.logspace(np.log10(left_edge), np.log10(right_edge), n_bins + 1), edges
        )

    @staticmethod
    def test_bin_centers_len():
        # arrange
        grid_size = 44
        sut = ppmc.BinGrid(grid_size, "log", 1, 100)

        # act
        centers = sut.centers

        # assert
        assert grid_size == len(centers)

    @staticmethod
    def test_bin_centers_values():
        # arrange
        n_bins = 10
        left_edge = 1
        right_edge = 100
        sut = ppmc.BinGrid(n_bins, "log", left_edge, right_edge)

        # act
        centers = sut.centers

        # assert
        np.testing.assert_array_almost_equal(
            np.logspace(np.log10(left_edge), np.log10(right_edge), 2 * n_bins + 1)[
                1:-1:2
            ],
            centers,
        )

    @staticmethod
    def test_invalid_grid():
        grid_size = 100
        try:
            _ = ppmc.BinGrid(grid_size, "X", 1, 100)
        except ValueError as error:
            assert str(error) == "Invalid grid spacing."

    @staticmethod
    def test_histogram_1d():
        # arrange
        n_data = 1000
        grid = ppmc.BinGrid(100, "linear", 0, 1000)
        vals = np.random.random(n_data) * 1000
        weights = np.ones(n_data)
        hist, bin_edges = np.histogram(vals, bins=grid.edges)

        # act
        data = ppmc.histogram_1d(grid, vals, weights)

        # assert
        np.testing.assert_array_almost_equal(data, hist / (bin_edges[1] - bin_edges[0]))

    @staticmethod
    def test_histogram_2d_linear_linear():
        # arrange
        n_data = 1000
        x_grid = ppmc.BinGrid(15, "linear", 0, 1000)
        y_grid = ppmc.BinGrid(12, "linear", 0, 500)
        x_vals = np.random.random(n_data) * 1000
        y_vals = np.random.random(n_data) * 500
        weights = np.random.random(n_data)
        data_numpy, bin_edges_x, bin_edges_y = np.histogram2d(
            x_vals, y_vals, bins=[x_grid.edges, y_grid.edges], weights=weights
        )
        cell_size = (bin_edges_x[1] - bin_edges_x[0]) * (
            bin_edges_y[1] - bin_edges_y[0]
        )

        # act
        data = ppmc.histogram_2d(x_grid, x_vals, y_grid, y_vals, weights)

        # assert
        np.testing.assert_array_almost_equal(
            np.array(data), data_numpy / cell_size, decimal=15
        )

    @staticmethod
    def test_histogram_2d_linear_log():
        # arrange
        n_data = 100
        y_data_min = 0.1
        y_data_max = 10.0
        x_grid = ppmc.BinGrid(15, "linear", 0, 1000)
        y_grid = ppmc.BinGrid(12, "log", y_data_min, y_data_max)
        x_vals = np.random.random(n_data) * 1000
        y_vals = y_data_min * 10 ** (
            np.log10(y_data_max / y_data_min) * np.random.random(n_data)
        )
        weights = np.random.random(n_data)
        data_numpy, bin_edges_x, bin_edges_y = np.histogram2d(
            x_vals, y_vals, bins=[x_grid.edges, y_grid.edges], weights=weights
        )
        cell_size = (bin_edges_x[1] - bin_edges_x[0]) * np.log(
            bin_edges_y[1] / bin_edges_y[0]
        )

        # act
        data = ppmc.histogram_2d(x_grid, x_vals, y_grid, y_vals, weights)

        # assert
        np.testing.assert_array_almost_equal(
            np.array(data), data_numpy / cell_size, decimal=15
        )

    @staticmethod
    def test_histogram_2d_log_log():
        # arrange
        n_data = 100
        x_data_min = 1
        x_data_max = 1000
        y_data_min = 0.1
        y_data_max = 10.0
        x_grid = ppmc.BinGrid(15, "log", x_data_min, x_data_max)
        y_grid = ppmc.BinGrid(12, "log", y_data_min, y_data_max)
        x_vals = x_data_min * 10 ** (
            np.log10(x_data_max / x_data_min) * np.random.random(n_data)
        )
        y_vals = y_data_min * 10 ** (
            np.log10(y_data_max / y_data_min) * np.random.random(n_data)
        )
        weights = np.random.random(n_data)
        data_numpy, bin_edges_x, bin_edges_y = np.histogram2d(
            x_vals, y_vals, bins=[x_grid.edges, y_grid.edges], weights=weights
        )
        cell_size = np.log(bin_edges_x[1] / bin_edges_x[0]) * np.log(
            bin_edges_y[1] / bin_edges_y[0]
        )

        # act
        data = ppmc.histogram_2d(x_grid, x_vals, y_grid, y_vals, weights)

        # assert
        np.testing.assert_array_almost_equal(
            np.array(data), data_numpy / cell_size, decimal=13
        )
