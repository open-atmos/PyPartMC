####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2025 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import numpy as np

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_dist import AERO_DIST_CTOR_ARG_MINIMAL


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
        grid_size = 400
        bin_grid = ppmc.BinGrid(grid_size, "log", 1e-9, 1e-5)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroBinned(aero_data, bin_grid)

        # act
        sut.add_aero_dist(bin_grid, aero_dist)
        num_conc = sut.num_conc

        # assert
        assert np.isclose(
            np.sum(np.array(num_conc) * np.array(bin_grid.widths)),
            aero_dist.num_conc,
            rtol=1e-6,
        )

    @staticmethod
    def test_vol_conc():
        # arrange
        grid_size = 400
        bin_grid = ppmc.BinGrid(grid_size, "log", 1e-9, 1e-5)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroBinned(aero_data, bin_grid)

        # act
        sut.add_aero_dist(bin_grid, aero_dist)
        vol_conc = sut.vol_conc

        # assert
        assert np.isclose(
            np.sum(np.array(vol_conc) * np.array(bin_grid.widths)),
            np.sum(
                np.array(sut.num_conc)
                * (4 * np.pi / 3)
                * np.array(bin_grid.centers) ** 3
            ),
            rtol=1e-6,
        )
