####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import copy
import gc

import numpy as np
import pytest

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_mode import AERO_MODE_CTOR_LOG_NORMAL, AERO_MODE_CTOR_LOG_NORMAL_FULL

AERO_DIST_CTOR_ARG_MINIMAL = [
    AERO_MODE_CTOR_LOG_NORMAL,
]

AERO_DIST_CTOR_ARG_FULL = [
    AERO_MODE_CTOR_LOG_NORMAL_FULL,
]


@pytest.fixture
def sut_minimal():
    aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
    sut = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
    aero_data = None
    gc.collect()
    return sut


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
        modes = {}
        for k in range(n_modes):
            mode_data["num_conc"] = num_concs[k]
            modes[k] = copy.deepcopy(mode_data)

        # act
        sut = ppmc.AeroDist(
            aero_data, [{f"mode_{k}": modes[k] for k in range(n_modes)}]
        )

        # assert
        assert sut.n_mode == n_modes
        assert sut.num_conc == np.sum(num_concs)
        for i in range(sut.n_mode):
            assert sut.mode(i).type == modes[i]["mode_type"]
            assert sut.mode(i).num_conc == modes[i]["num_conc"]
            assert sut.mode(i).name == f"mode_{i}"

    @staticmethod
    @pytest.mark.parametrize("idx", (-1, 500))
    def test_get_mode_out_of_range(
        sut_minimal, idx
    ):  # pylint: disable=redefined-outer-name
        # act
        try:
            _ = sut_minimal.mode(idx)
        except IndexError:
            return

        # assert
        assert False

    @staticmethod
    def test_get_mode_result_lifetime(
        sut_minimal,
    ):  # pylint: disable=redefined-outer-name
        # arrange
        mode = sut_minimal.mode(0)
        mode_type = mode.type

        # act
        sut_minimal = None
        gc.collect()

        # assert
        assert mode.type == mode_type

    @staticmethod
    def test_get_mode_is_a_copy(sut_minimal):  # pylint: disable=redefined-outer-name
        # arrange
        new_type = "mono"
        mode_idx = 0
        mode = sut_minimal.mode(mode_idx)
        assert mode.type != new_type

        # act
        mode.type = new_type

        # assert
        assert sut_minimal.mode(mode_idx).type != new_type
