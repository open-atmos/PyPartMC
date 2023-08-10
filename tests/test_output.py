####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import os

import numpy as np

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_FULL
from .test_aero_dist import AERO_DIST_CTOR_ARG_COAGULATION
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_run_part_opt import RUN_PART_OPT_CTOR_ARG_SIMULATION
from .test_scenario import SCENARIO_CTOR_ARG_SIMULATION


class TestOutput:
    @staticmethod
    def test_output_netcdf(tmp_path):
        filename = tmp_path / "test"

        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        gas_state = ppmc.GasState(gas_data)
        scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_SIMULATION)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        scenario.init_env_state(env_state, 0.0)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_COAGULATION)
        n_part = 100
        aero_state = ppmc.AeroState(aero_data, n_part, "nummass_source")
        aero_state.dist_sample(
            aero_dist,
            sample_prop=1.0,
            create_time=0.0,
            allow_doubling=True,
            allow_halving=True,
        )

        ppmc.output_state(
            str(filename), aero_data, aero_state, gas_data, gas_state, env_state
        )

        assert os.path.exists(str(filename) + "_0001_00000001.nc")

    @staticmethod
    def test_input_netcdf(tmp_path):
        filename = tmp_path / "test"

        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        gas_state = ppmc.GasState(gas_data)
        scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_SIMULATION)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        scenario.init_env_state(env_state, 0.0)
        run_part_opt = ppmc.RunPartOpt(
            {**RUN_PART_OPT_CTOR_ARG_SIMULATION, "output_prefix": str(filename)}
        )
        camp_core = ppmc.CampCore()
        photolysis = ppmc.Photolysis()
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_COAGULATION)
        n_part = 100
        aero_state = ppmc.AeroState(aero_data, n_part, "nummass_source")
        aero_state.dist_sample(
            aero_dist,
            sample_prop=1.0,
            create_time=0.0,
            allow_doubling=True,
            allow_halving=True,
        )

        num_concs = aero_state.num_concs
        mix_rats = gas_state.mix_rats
        ppmc.run_part(
            scenario,
            env_state,
            aero_data,
            aero_state,
            gas_data,
            gas_state,
            run_part_opt,
            camp_core,
            photolysis,
        )

        assert aero_state.num_concs != num_concs

        ppmc.input_state(
            str(filename) + "_0001_00000001.nc",
            aero_data,
            aero_state,
            gas_data,
            gas_state,
            env_state,
        )

        # check an integer
        # check a real
        assert len(num_concs) == len(aero_state)
        np.testing.assert_allclose(np.array(aero_state.num_concs), np.array(num_concs))
        np.testing.assert_array_equal(gas_state.mix_rats, mix_rats)
