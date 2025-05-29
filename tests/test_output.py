####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022-2025 University of Illinois Urbana-Champaign                                  #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import os

import numpy as np

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_FULL, AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_dist import (
    AERO_DIST_CTOR_ARG_COAGULATION,
    AERO_DIST_CTOR_ARG_EXP,
    AERO_DIST_CTOR_ARG_MINIMAL,
)
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_run_exact_opt import RUN_EXACT_OPT_CTOR_ARG_SIMULATION
from .test_run_part_opt import RUN_PART_OPT_CTOR_ARG_SIMULATION
from .test_run_sect_opt import RUN_SECT_OPT_CTOR_ARG_SIMULATION
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL, SCENARIO_CTOR_ARG_SIMULATION


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
            allow_doubling=False,
            allow_halving=False,
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

        aero_data, aero_state, gas_data, gas_state, env_state = ppmc.input_state(
            str(filename) + "_0001_00000001.nc"
        )

        # check an integer
        # check a real
        assert len(num_concs) == len(aero_state)
        np.testing.assert_allclose(np.array(aero_state.num_concs), np.array(num_concs))
        np.testing.assert_array_equal(gas_state.mix_rats, mix_rats)

    @staticmethod
    def test_input_sectional_netcdf(tmp_path):
        bin_grid = ppmc.BinGrid(100, "log", 1e-9, 1e-5)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        scenario.init_env_state(env_state, 0.0)
        filename = tmp_path / "test"
        run_sect_opt = ppmc.RunSectOpt(
            {**RUN_SECT_OPT_CTOR_ARG_SIMULATION, "output_prefix": str(filename)},
            env_state,
        )
        ppmc.run_sect(
            bin_grid,
            gas_data,
            aero_data,
            aero_dist,
            scenario,
            env_state,
            run_sect_opt,
        )

        assert os.path.exists(str(filename) + "_00000001.nc")

        aero_data, bin_grid, aero_binned, gas_data, _, env_state = ppmc.input_sectional(
            str(filename) + "_00000001.nc"
        )

        assert np.isclose(
            np.sum(np.array(aero_binned.num_conc) * np.array(bin_grid.widths)),
            aero_dist.num_conc,
        )

    @staticmethod
    def test_input_exact_netcdf(tmp_path):
        bin_grid = ppmc.BinGrid(100, "log", 1e-9, 1e-3)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_EXP)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        scenario.init_env_state(env_state, 0.0)
        filename = tmp_path / "test"
        run_exact_opt = ppmc.RunExactOpt(
            {**RUN_EXACT_OPT_CTOR_ARG_SIMULATION, "output_prefix": str(filename)},
            env_state,
        )
        ppmc.run_exact(
            bin_grid,
            gas_data,
            aero_data,
            aero_dist,
            scenario,
            env_state,
            run_exact_opt,
        )

        assert os.path.exists(str(filename) + "_00000001.nc")

        aero_data, bin_grid, aero_binned, gas_data, _, env_state = ppmc.input_exact(
            str(filename) + "_00000001.nc"
        )

        assert np.isclose(
            np.sum(np.array(aero_binned.num_conc) * np.array(bin_grid.widths)),
            aero_dist.num_conc,
        )
