####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022-2024 University of Illinois Urbana-Champaign                                  #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc

import numpy as np
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
    def test_latitude():
        # arrange
        sut = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        value = 40.0

        # act
        sut.latitude = value

        # assert
        assert value == sut.latitude

    @staticmethod
    def test_longitude():
        # arrange
        sut = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        value = 180.0

        # act
        sut.longitude = value

        # assert
        assert value == sut.longitude

    @staticmethod
    def test_altitude():
        # arrange
        sut = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        value = 200.0

        # act
        sut.altitude = value

        # assert
        assert value == sut.altitude

    @staticmethod
    def test_additive_kernel_coefficient():
        # arrange
        sut = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        value = 1500.0

        # act
        sut.additive_kernel_coefficient = value

        # assert
        assert value == sut.additive_kernel_coefficient

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

    @staticmethod
    def test_air_molar_density():
        # arrange
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        scenario.init_env_state(env_state, 0.0)

        # assert
        assert (
            1 * si.mol / si.m**3 < env_state.air_molar_density < 100 * si.mol / si.m**3
        )

    @staticmethod
    def test_conc_ppb_conversions():
        # arrange
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        scenario.init_env_state(env_state, 0.0)

        # act
        conc_orig = 1.0
        ppb = env_state.conc_to_ppb(conc_orig)
        ppb_orig = 1.0
        conc = env_state.ppb_to_conc(ppb_orig)

        # assert
        assert ppb < conc_orig
        assert ppb_orig < conc
        np.testing.assert_almost_equal(env_state.ppb_to_conc(ppb), conc_orig)
        np.testing.assert_almost_equal(env_state.conc_to_ppb(conc), ppb_orig)

    @staticmethod
    def test_sat_vapor_pressure():
        # arrange
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        scenario.init_env_state(env_state, 0.0)

        # assert
        assert env_state.sat_vapor_pressure > 0.0
