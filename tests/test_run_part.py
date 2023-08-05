####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import pytest

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_state import AERO_STATE_CTOR_ARG_MINIMAL
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_run_part_opt import RUN_PART_OPT_CTOR_ARG_MINIMAL
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL


@pytest.fixture
def common_args():
    aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
    gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
    gas_state = ppmc.GasState(gas_data)
    scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
    env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
    scenario.init_env_state(env_state, 0.0)
    return (
        scenario,
        env_state,
        aero_data,
        ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL),
        gas_data,
        gas_state,
        ppmc.RunPartOpt(RUN_PART_OPT_CTOR_ARG_MINIMAL),
        ppmc.CampCore(),
        ppmc.Photolysis(),
    )


class TestRunPart:
    @staticmethod
    def test_run_part(common_args):  # pylint: disable=redefined-outer-name
        ppmc.run_part(*common_args)

    @staticmethod
    def test_run_part_timestep(common_args):  # pylint: disable=redefined-outer-name
        ppmc.run_part_timestep(*common_args, 0, 0)

    @staticmethod
    def test_run_part_timeblock(common_args):  # pylint: disable=redefined-outer-name
        ppmc.run_part_timeblock(*common_args, 0, 0, 0)
