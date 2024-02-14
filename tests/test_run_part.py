####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import numpy as np
import pytest

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_FULL, AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_dist import AERO_DIST_CTOR_ARG_FULL
from .test_aero_state import AERO_STATE_CTOR_ARG_MINIMAL
from .test_env_state import ENV_STATE_CTOR_ARG_HIGH_RH, ENV_STATE_CTOR_ARG_MINIMAL
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_run_part_opt import RUN_PART_OPT_CTOR_ARG_SIMULATION
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL


@pytest.fixture(name="common_args")
def common_args_fixture(tmp_path):
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
    def test_run_part(common_args):
        ppmc.run_part(*common_args)

        assert common_args[1].elapsed_time == RUN_PART_OPT_CTOR_ARG_SIMULATION["t_max"]

    @staticmethod
    def test_run_part_timestep(common_args):
        (last_output_time, last_progress_time, i_output) = ppmc.run_part_timestep(
            *common_args, 1, 0, 0, 0, 1
        )

        assert common_args[1].elapsed_time == RUN_PART_OPT_CTOR_ARG_SIMULATION["del_t"]
        assert last_output_time == 0.0
        assert last_progress_time == 0.0
        assert i_output == 1

    @staticmethod
    def test_run_part_timeblock(common_args):
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

    @staticmethod
    def test_run_part_do_condensation(common_args, tmp_path):
        filename = tmp_path / "test"
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_HIGH_RH)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_FULL)
        aero_state = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
        args = list(common_args)
        args[0].init_env_state(env_state, 0.0)
        args[1] = env_state
        args[2] = aero_data
        args[3] = aero_state
        args[6] = ppmc.RunPartOpt(
            {
                **RUN_PART_OPT_CTOR_ARG_SIMULATION,
                "output_prefix": str(filename),
                "do_condensation": True,
            }
        )
        aero_state.dist_sample(aero_dist, 1.0, 0.0, True, True)
        ppmc.condense_equilib_particles(env_state, aero_data, aero_state)
        ppmc.run_part(*args)

        assert np.sum(aero_state.masses(include=["H2O"])) > 0.0
