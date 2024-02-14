####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022-2024 University of Illinois Urbana-Champaign                                  #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc

import pytest

import PyPartMC as ppmc
from PyPartMC import si

from .common import ENV_STATE_CTOR_ARG_MINIMAL
from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL

ENV_STATE_CTOR_ARG_HIGH_RH = {**ENV_STATE_CTOR_ARG_MINIMAL}
ENV_STATE_CTOR_ARG_HIGH_RH["rel_humidity"] = 0.95


class TestEnvState:
    @staticmethod
    @pytest.mark.parametrize("ctor_arg", (ENV_STATE_CTOR_ARG_MINIMAL,))
    def test_ctor(ctor_arg):
        # arrange
        pass

        # act
        sut = ppmc.EnvState(ctor_arg)

        # assert
        assert sut is not None

    @staticmethod
    def test_dtor():
        # arrange
        sut = ppmc.EnvState(  # pylint: disable=unused-variable
            ENV_STATE_CTOR_ARG_MINIMAL
        )
        gc.collect()

        # act
        sut = None
        gc.collect()

        # assert
        pass

    @staticmethod
    def test_height():
        # arrange
        sut = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        value = 1

        # act
        sut.height = value

        # assert
        assert value == sut.height

    @staticmethod
    def test_pressure():
        # arrange
        sut = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        value = 101325

        # act
        sut.pressure = value

        # assert
        assert value == sut.pressure

    @staticmethod
    def test_humidity_ctor():
        # arrange and act
        sut = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)

        # assert
        assert ENV_STATE_CTOR_ARG_MINIMAL["rel_humidity"] == sut.rh

    @staticmethod
    def test_elapsed_time():
        # arrange and act
        sut = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)

        # assert
        assert sut.elapsed_time == 0

    @staticmethod
    def test_start_time():
        # arrange and act
        sut = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)

        # assert
        assert sut.start_time == ENV_STATE_CTOR_ARG_MINIMAL["start_time"]

    @staticmethod
    def test_air_density():
        # arrange
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        scenario.init_env_state(env_state, 0.0)

        # assert
        assert 1 * si.kg / si.m**3 < env_state.air_density < 1.5 * si.kg / si.m**3
