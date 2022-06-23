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
            np.logspace(np.log10(left_edge), np.log10(right_edge), n_bins+1),
            edges
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
            np.logspace(np.log10(left_edge), np.log10(right_edge), 2*n_bins+1)[1:-1:2],
            centers
        )

    @staticmethod
    def test_invalid_grid():
        grid_size = 100
        try:
            _ = ppmc.BinGrid(grid_size, "X", 1, 100)
        except ValueError as error:
            assert str(error) == "Invalid grid spacing."
