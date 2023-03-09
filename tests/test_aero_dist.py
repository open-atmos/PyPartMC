####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import copy

import numpy as np
import pytest

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_mode import AERO_MODE_CTOR_LOG_NORMAL

AERO_DIST_CTOR_ARG_MINIMAL = [
    AERO_MODE_CTOR_LOG_NORMAL,
]


# pylint: disable=too-few-public-methods
class TestAeroDist:
    @staticmethod
    def test_ctor():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act
        sut = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)

        # assert
        assert sut is not None

    @staticmethod
    @pytest.mark.parametrize("n_modes", (1, 2, 3))
    def test_ctor_multimode(n_modes):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        mode_data = AERO_DIST_CTOR_ARG_MINIMAL[0]["test_mode"]
        num_concs = np.random.rand(n_modes)
        aero_dists = {}
        for k in range(n_modes):
            mode_data["num_conc"] = num_concs[k]
            aero_dists[k] = copy.deepcopy(mode_data)

        # act
        sut = ppmc.AeroDist(
            aero_data, [{f"mode_{k}": aero_dists[k] for k in range(n_modes)}]
        )

        # assert
        assert sut.n_mode == n_modes
        assert sut.num_conc == np.sum(num_concs)
