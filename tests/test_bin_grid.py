####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import PyPartMC as ppmc
import numpy as np

BIN_GRID_N_BIN = 100

class TestBinGrid:
    @staticmethod
    def test_ctor():
        # arrange
        pass

        # act
        sut = ppmc.BinGrid(BIN_GRID_N_BIN, "log", 1, 100)

        # assert
        assert sut is not None

    @staticmethod
    def test_len():
        # arrange
        sut = ppmc.BinGrid(BIN_GRID_N_BIN, "log", 1, 100)
        # act
        size = len(sut)

        # assert
        assert size == BIN_GRID_N_BIN

    def test_bin_edges():
        sut = ppmc.BinGrid(BIN_GRID_N_BIN, "log", 1, 100)    

        size = len(sut.edges)

        assert size == BIN_GRID_N_BIN + 1 

    def test_bin_centers():
        sut = ppmc.BinGrid(BIN_GRID_N_BIN, "log", 1, 100)

        size = len(sut.centers)

        assert size == BIN_GRID_N_BIN
