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
from .test_run_part_opt import RUN_PART_OPT_CTOR_ARG_SIMULATION
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL


@pytest.fixture
def common_args(tmp_path):
    aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
    gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
    gas_state = ppmc.GasState(gas_data)
    scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
    env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
    scenario.init_env_state(env_state, 0.0)
    filename = tmp_path / "test"
    run_part_opt = ppmc.RunPartOpt(
        {**RUN_PART_OPT_CTOR_ARG_SIMULATION, "output_prefix": str(filename)}
    )
    return (
        scenario,
        env_state,
        aero_data,
        ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL),
        gas_data,
        gas_state,
        run_part_opt,
        ppmc.CampCore(),
        ppmc.Photolysis(),
    )


class TestRunPart:
    @staticmethod
    def test_run_part(common_args):  # pylint: disable=redefined-outer-name
        ppmc.run_part(*common_args)

        assert common_args[1].elapsed_time == RUN_PART_OPT_CTOR_ARG_SIMULATION["t_max"]

    @staticmethod
    def test_run_part_timestep(common_args):  # pylint: disable=redefined-outer-name
        (last_output_time, last_progress_time, i_output) = ppmc.run_part_timestep(
            *common_args, 1, 0, 0, 0, 1
        )

        assert common_args[1].elapsed_time == RUN_PART_OPT_CTOR_ARG_SIMULATION["del_t"]
        assert last_output_time == 0.0
        assert last_progress_time == 0.0
        assert i_output == 1

    @staticmethod
    def test_run_part_timeblock(common_args):  # pylint: disable=redefined-outer-name

        num_times = int(
            RUN_PART_OPT_CTOR_ARG_SIMULATION["t_output"]
            / RUN_PART_OPT_CTOR_ARG_SIMULATION["del_t"]
        )
        (last_output_time, last_progress_time, i_output) = ppmc.run_part_timeblock(
            *common_args, 1, num_times, 0, 0, 0, 1
        )
        assert last_output_time == RUN_PART_OPT_CTOR_ARG_SIMULATION["t_output"]
        assert last_progress_time == 0.0
        assert i_output == 2
