####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

from cmath import nan

import numpy as np
import pytest

import PyPartMC as ppmc
from PyPartMC import si

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL


class TestLossRate:
    @staticmethod
    @pytest.mark.parametrize(
        "loss_function_params",
        (
            {"loss_function": "none"},
            {"loss_function": "constant"},
            {"loss_function": "volume"},
            {"loss_function": "drydep"},
            {
                "loss_function": "chamber",
                "chamber_vol": 84.3 * si.m**3,
                "area_diffuse": 103 * si.m**2,
                "area_sedi": 12.6 * si.m**2,
                "prefactor_BL": 0.005 * si.m,
                "exponent_BL": 0.274,
            },
        ),
    )
    def test_loss_rate(loss_function_params: str):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        scenario_ctor_arg = {**SCENARIO_CTOR_ARG_MINIMAL, **loss_function_params}
        scenario = ppmc.Scenario(gas_data, aero_data, scenario_ctor_arg)
        vol = (4 / 3) * np.pi * (1e-6) ** 3
        density = 1
        env_state.height = 1
        env_state.set_temperature(300)
        env_state.pressure = 101325
        aero_data.frac_dim = 3
        aero_data.prime_radius = 1e-8
        aero_data.vol_fill_factor = 1

        # act
        rate = ppmc.loss_rate(scenario, vol, density, aero_data, env_state)

        # assert
        assert rate is not nan

    @staticmethod
    def test_loss_rate_dry_dep():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        vol = (4 / 3) * np.pi * (1e-6) ** 3
        density = 1
        env_state.height = 1
        env_state.set_temperature(300)
        env_state.pressure = 101325
        aero_data.frac_dim = 3
        aero_data.prime_radius = 1e-8
        aero_data.vol_fill_factor = 1

        # act
        rate = ppmc.loss_rate_dry_dep(vol, density, aero_data, env_state)

        # assert
        assert rate is not nan
