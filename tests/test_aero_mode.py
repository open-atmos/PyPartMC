####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL

AERO_MODE_CTOR_LOG_NORMAL = {}


class TestAeroMode:
    @staticmethod
    def test_ctor():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)

        # assert
        assert sut is not None

    @staticmethod
    def test_set_vol_frac():
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        # act
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)
        vals = [1.0]
        sut.vol_frac = vals

        assert sut.vol_frac == vals

    @staticmethod
    def test_set_num_conc():
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        # act
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)
        val = 1234
        sut.num_conc = val

        assert sut.num_conc == val

    @staticmethod
    def test_set_gsd():
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        # act
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)
        val = 1.234
        sut.gsd = val

        assert sut.gsd == val

    @staticmethod
    def test_set_char_radius():
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        # act
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)
        val = 1e-9
        sut.char_radius = val

        assert sut.char_radius == val

    @staticmethod
    def test_set_vol_frac_invalid():
        # act
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        # act
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)
        try:
            sut.vol_frac = [1.0, 0.0]
        except RuntimeError:
            return

        assert False

    @staticmethod
    def test_set_sample_invalid():
        # act
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        # act
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)
        try:
            sut.set_sample([1, 10], [10, 10])
        except RuntimeError:
            return

        assert False
