####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import pytest
import numpy as np

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL, AERO_DATA_CTOR_ARG_FULL

AERO_STATE_CTOR_ARG_MINIMAL = 44


class TestAeroState:
    @staticmethod
    def test_ctor():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act
        sut = ppmc.AeroState(AERO_STATE_CTOR_ARG_MINIMAL, aero_data)

        # assert
        assert sut is not None

    @staticmethod
    @pytest.mark.xfail(strict=True)  # TODO #116
    @pytest.mark.parametrize("n_part", (1, 44, 666))
    def test_len(n_part):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroState(n_part, aero_data)

        # act
        size = len(sut)

        # assert
        assert size == n_part

    @staticmethod
    def test_copy():
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)
        # act
        sut = ppmc.AeroState(AERO_STATE_CTOR_ARG_MINIMAL, aero_data)

        aero_state_copy = sut.__deepcopy__(aero_data)

        # assert
        assert aero_state_copy is not sut
        assert np.sum(sut.diameters(aero_data)) ==  \
            np.sum(aero_state_copy.diameters(aero_data))
