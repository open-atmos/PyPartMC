####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import platform

import numpy as np
import pytest

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_dist import AERO_DIST_CTOR_ARG_MINIMAL
from .test_env_state import ENV_STATE_CTOR_ARG_HIGH_RH, ENV_STATE_CTOR_ARG_MINIMAL
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_run_sect_opt import RUN_SECT_OPT_CTOR_ARG_SIMULATION
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL

@pytest.fixture(name="common_args")
def common_args_fixture(tmp_path):
    bin_grid = ppmc.BinGrid(100, 'log', 1e-9,1e-5)
    aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
    aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
    gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
    gas_state = ppmc.GasState(gas_data)
    scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
    env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
    scenario.init_env_state(env_state, 0.0)
    filename = tmp_path / "test"
    run_sect_opt = ppmc.RunSectOpt(
        {**RUN_SECT_OPT_CTOR_ARG_SIMULATION, "output_prefix": str(filename)}
    )
    return (
        bin_grid,
        gas_data,
        aero_data,
        aero_dist,
        scenario,
        env_state,
        run_sect_opt,
    )

class TestRunPart:
    @staticmethod
    def test_run_sect(common_args):
        ppmc.run_sect(*common_args)

        assert common_args[5].elapsed_time == RUN_SECT_OPT_CTOR_ARG_SIMULATION["t_max"]
