####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import platform

import numpy as np
import pytest

import PyPartMC as ppmc
from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_dist import (
    AERO_DIST_CTOR_ARG_MINIMAL,
)

class TestAeroBinned:
    @staticmethod
    def test_ctor():
        # act
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroBinned(aero_data)

        # assert
        assert sut is not None

    @staticmethod
    def test_num_conc():
        # arrange
        grid_size = 666
        bin_grid = ppmc.BinGrid(grid_size, "log", 1e-9, 1e-5)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroBinned(aero_data, bin_grid)

        # act
        sut.add_aero_dist(bin_grid, aero_dist)
        num_conc = sut.num_conc

        # assert
        assert np.isclose(np.sum(np.array(num_conc) * np.array(bin_grid.widths)),
            aero_dist.num_conc, rtol=1e-6)
