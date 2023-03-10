####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import copy
import gc
import json

import pytest

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_mode import AERO_MODE_CTOR_LOG_NORMAL
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL

SCENARIO_CTOR_ARG_MINIMAL = {
    "temp_profile": [{"time": [0]}, {"temp": [273]}],
    "pressure_profile": [{"time": [0]}, {"pressure": [1e5]}],
    "height_profile": [{"time": [0]}, {"height": [1]}],
    "gas_emissions": [{"time": [0]}, {"rate": [0]}, {"SO2": [0]}],
    "gas_background": [{"time": [0]}, {"rate": [0]}, {"SO2": [0]}],
    "aero_emissions": [
        {"time": [0]},
        {"rate": [0]},
        {"dist": [AERO_MODE_CTOR_LOG_NORMAL]},
    ],
    "aero_background": [
        {"time": [0]},
        {"rate": [0]},
        {"dist": [AERO_MODE_CTOR_LOG_NORMAL]},
    ],
    "loss_function": "none",
}


class TestScenario:
    @staticmethod
    @pytest.mark.parametrize(
        "params",
        (
            {
                "gas_data_ctor_arg": GAS_DATA_CTOR_ARG_MINIMAL,
                "scenario_ctor_arg": SCENARIO_CTOR_ARG_MINIMAL,
            },
            {
                "gas_data_ctor_arg": ("SO2", "NO2"),
                "scenario_ctor_arg": {
                    "temp_profile": [{"time": [0, 1, 2]}, {"temp": [1, 2, 3]}],
                    "pressure_profile": [{"time": [0, 1, 2]}, {"pressure": [1, 2, 3]}],
                    "height_profile": [
                        {"time": [0, 1, 2, 3, 4]},  # TODO #115: should be caught
                        {"height": [1, 2, 3]},
                    ],
                    "gas_emissions": [
                        {"time": [0, 1, 2]},
                        {"rate": [1, 1, 1]},
                        {"SO2": [0, 0, 0]},
                        {"NO2": [0, 0, 0]},
                    ],
                    "gas_background": [
                        {"time": [0, 1, 2]},
                        {"rate": [1, 1, 1]},
                        {"SO2": [0, 0, 0]},
                        {"NO2": [0, 0, 0]},
                    ],
                    "aero_emissions": [
                        {"time": [0]},
                        {"rate": [0]},
                        {"dist": [AERO_MODE_CTOR_LOG_NORMAL]},
                    ],
                    "aero_background": [
                        {"time": [0]},
                        {"rate": [0]},
                        {"dist": [AERO_MODE_CTOR_LOG_NORMAL]},
                    ],
                    "loss_function": "none",
                },
            },
        ),
    )
    def test_ctor(params):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        gas_data = ppmc.GasData(params["gas_data_ctor_arg"])

        # act
        sut = ppmc.Scenario(gas_data, aero_data, params["scenario_ctor_arg"])

        # assert
        assert sut is not None

    @staticmethod
    def test_dtor():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        # pylint: disable=unused-variable
        sut = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
        gc.collect()

        # act
        sut = None
        gc.collect()

        # assert
        pass

    @staticmethod
    def test_str():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)

        # act
        json_actual = json.loads(str(sut))

        # assert
        assert json_actual == SCENARIO_CTOR_ARG_MINIMAL

    @staticmethod
    def test_init_env_state():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        time = 666.0
        sut = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)

        # act
        sut.init_env_state(env_state, time)

    @staticmethod
    @pytest.mark.xfail(strict=True)
    @pytest.mark.skipif("sys.platform != 'linux'")
    def test_ctor_fails_with_no_values_in_time_array():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        ctor_arg = copy.deepcopy(SCENARIO_CTOR_ARG_MINIMAL)
        ctor_arg["temp_profile"][0]["time"] = []

        # act
        _ = ppmc.Scenario(gas_data, aero_data, ctor_arg)

    @staticmethod
    def test_multi_mode():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        scenario_ctor_arg = copy.deepcopy(SCENARIO_CTOR_ARG_MINIMAL)
        for entry in ("aero_emissions", "aero_background"):
            scenario_ctor_arg[entry][-1]["dist"] = [
                {"A": AERO_MODE_CTOR_LOG_NORMAL["test_mode"]},
                {"B": AERO_MODE_CTOR_LOG_NORMAL["test_mode"]},
            ]

        # act
        _ = ppmc.Scenario(gas_data, aero_data, scenario_ctor_arg)
