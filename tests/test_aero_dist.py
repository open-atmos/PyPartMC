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
from PyPartMC import si

from .test_aero_data import AERO_DATA_CTOR_ARG_FULL, AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_mode import (
    AERO_MODE_CTOR_LOG_NORMAL,
    AERO_MODE_CTOR_LOG_NORMAL_COAGULATION,
    AERO_MODE_CTOR_LOG_NORMAL_FULL,
)

AERO_DIST_CTOR_ARG_MINIMAL = [
    AERO_MODE_CTOR_LOG_NORMAL,
]

AERO_DIST_CTOR_ARG_FULL = [
    AERO_MODE_CTOR_LOG_NORMAL_FULL,
]

AERO_DIST_CTOR_ARG_COAGULATION = [
    AERO_MODE_CTOR_LOG_NORMAL_COAGULATION,
]

AERO_DIST_CTOR_ARG_AVERAGE = [
    {
        "test_mode_A": {
            "mass_frac": [{"SO4": [1]}],
            "diam_type": "geometric",
            "mode_type": "mono",
            "num_conc": 1e12 / si.m**3,
            "diam": 2 * si.um,
        },
        "test_mode_B": {
            "mass_frac": [{"BC": [1]}],
            "diam_type": "geometric",
            "mode_type": "mono",
            "num_conc": 1e12 / si.m**3,
            "diam": 0.2 * si.um,
        },
    }
]


@pytest.fixture(name="sut_minimal")
def sut_minimal_fixture():
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
    @pytest.mark.parametrize("n_modes", (2, 3))
    @pytest.mark.parametrize(
        "order",
        (
            range,
            pytest.param(
                lambda n_modes: reversed(range(n_modes)),
                marks=(pytest.mark.xfail(strict=True),),
            ),  # TODO #213
        ),
    )
    def test_ctor_multimode(n_modes, order):
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
            aero_data, [{f"mode_{k}": modes[k] for k in order(n_modes)}]
        )

        # assert
        assert sut.n_mode == n_modes
        assert sut.num_conc == np.sum(num_concs)
        for i in range(sut.n_mode):
            assert sut.mode(i).type == modes[i]["mode_type"]
            assert sut.mode(i).num_conc == modes[i]["num_conc"]
            assert sut.mode(i).name == f"mode_{tuple(order(n_modes))[i]}"

    @staticmethod
    def test_ctor_modes_in_order(n_modes=4):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        mode_data = AERO_DIST_CTOR_ARG_MINIMAL[0]["test_mode"]
        num_concs = np.random.rand(n_modes)
        modes = {}
        for k in range(n_modes):
            mode_data["num_conc"] = num_concs[k]
            modes[k] = copy.deepcopy(mode_data)
        mode_map = {f"mode_{k}": modes[k] for k in range(n_modes)}

        # act
        sut = ppmc.AeroDist(aero_data, [mode_map])

        # assert
        expected_modes = tuple(mode_map.keys())
        actual_modes = tuple(sut.mode(i).name for i in range(sut.n_mode))
        assert expected_modes == actual_modes

    @staticmethod
    @pytest.mark.parametrize("idx", (-1, 500))
    def test_get_mode_out_of_range(sut_minimal, idx):
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
    ):
        # arrange
        mode = sut_minimal.mode(0)
        mode_type = mode.type

        # act
        sut_minimal = None
        gc.collect()

        # assert
        assert mode.type == mode_type

    @staticmethod
    def test_get_mode_is_a_copy(sut_minimal):
        # arrange
        new_type = "mono"
        mode_idx = 0
        mode = sut_minimal.mode(mode_idx)
        assert mode.type != new_type

        # act
        mode.type = new_type

        # assert
        assert sut_minimal.mode(mode_idx).type != new_type

    @staticmethod
    def test_get_source_names():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)
        _ = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_AVERAGE)

        # act
        sources = aero_data.sources

        # assert
        keys = tuple(AERO_DIST_CTOR_ARG_AVERAGE[0].keys())
        for i, key in enumerate(keys):
            assert sources[i] == key  # pylint: disable=unsubscriptable-object

    @staticmethod
    def test_ctor_multimode_error_on_repeated_mode_names():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act
        with pytest.raises(Exception) as exc_info:
            ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL * 2)

        # assert
        assert str(exc_info.value) == "Mode names must be unique"

    @staticmethod
    def test_ctor_error_on_repeated_massfrac_keys():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        fishy_ctor_arg = copy.deepcopy(AERO_DIST_CTOR_ARG_MINIMAL)
        fishy_ctor_arg[0]["test_mode"]["mass_frac"].append(
            fishy_ctor_arg[0]["test_mode"]["mass_frac"][0]
        )

        # act
        with pytest.raises(Exception) as exc_info:
            ppmc.AeroDist(aero_data, fishy_ctor_arg)

        # assert
        assert str(exc_info.value) == "mass_frac keys must be unique"
