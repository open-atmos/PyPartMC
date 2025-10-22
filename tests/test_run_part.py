####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import copy
import platform

import numpy as np
import pytest

import PyPartMC as ppmc

from .test_aero_data import (
    AERO_DATA_CTOR_ARG_FREEZING,
    AERO_DATA_CTOR_ARG_FULL,
    AERO_DATA_CTOR_ARG_MINIMAL,
)
from .test_aero_dist import AERO_DIST_CTOR_ARG_FREEZING, AERO_DIST_CTOR_ARG_FULL
from .test_aero_mode import AERO_MODE_CTOR_CAMP
from .test_aero_state import AERO_STATE_CTOR_ARG_MINIMAL
from .test_camp_core import CAMP_INPUT_PATH, chdir
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
        last_output_time, last_progress_time, i_output = ppmc.run_part_timestep(
            *common_args, 1, 0, 0, 0, 1
        )

        assert common_args[1].elapsed_time == RUN_PART_OPT_CTOR_ARG_SIMULATION["del_t"]
        assert last_output_time == 0.0
        assert last_progress_time == 0.0
        assert i_output == 1

    @staticmethod
    def test_run_part_timeblock(common_args):
        # arrange
        num_times = int(
            RUN_PART_OPT_CTOR_ARG_SIMULATION["t_output"]
            / RUN_PART_OPT_CTOR_ARG_SIMULATION["del_t"]
        )

        # act
        last_output_time, last_progress_time, i_output = ppmc.run_part_timeblock(
            *common_args, 1, num_times, 0, 0, 0, 1
        )

        # assert
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
        aero_state.dist_sample(aero_dist, 1.0, 0.0, False, False)
        ppmc.condense_equilib_particles(env_state, aero_data, aero_state)
        ppmc.run_part(*args)

        assert np.sum(aero_state.masses(include=["H2O"])) > 0.0

    @staticmethod
    @pytest.mark.parametrize(
        "flags",
        (
            ((True, True), (True, False)),
            ((True, True), (False, True)),
            ((True, True), (False, False)),
            ((False, False), (True, False)),
            ((False, False), (False, True)),
            ((False, False), (True, True)),
            ((True, False), (False, False)),
            ((True, False), (False, True)),
            ((False, True), (False, False)),
            ((False, True), (True, False)),
        ),
    )
    @pytest.mark.parametrize(
        "fun_args",
        (
            ("run_part", []),
            ("run_part_timestep", [0, 0, 0, 0, 0]),
            ("run_part_timeblock", [0, 0, 0, 0, 0, 0]),
        ),
    )
    @pytest.mark.skipif(platform.machine() == "arm64", reason="TODO #348")
    def test_run_part_allow_flag_mismatch(common_args, tmp_path, fun_args, flags):
        # arrange
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
                "allow_doubling": flags[0][0],
                "allow_halving": flags[0][1],
            }
        )
        aero_state.dist_sample(aero_dist, 1.0, 0.0, flags[1][0], flags[1][1])

        # act
        with pytest.raises(RuntimeError) as excinfo:
            getattr(ppmc, fun_args[0])(*args, *fun_args[1])

        # assert
        assert (
            str(excinfo.value)
            == "allow halving/doubling flags set differently then while sampling"
        )

    @staticmethod
    @pytest.mark.skipif(
        "site-packages" in ppmc.__file__, reason="Skipped for wheel install"
    )
    @pytest.mark.parametrize(
        "run_part_variant, variant_args",
        (
            ("run_part", ()),
            ("run_part_timeblock", (1, 10, 0, 0, 0, 1)),
            ("run_part_timestep", (1, 0, 0, 0, 1)),
        ),
    )
    def test_run_part_with_camp_assuming_installed_in_editable_mode_from_checkout(
        tmp_path, run_part_variant, variant_args
    ):
        # arrange
        assert CAMP_INPUT_PATH.exists()
        with chdir(CAMP_INPUT_PATH):
            camp_core = ppmc.CampCore("config.json")

        filename = tmp_path / "test"
        aero_data = ppmc.AeroData(camp_core)
        gas_data = ppmc.GasData(camp_core)
        gas_state = ppmc.GasState(gas_data)

        scenario_ctor_arg = copy.deepcopy(SCENARIO_CTOR_ARG_MINIMAL)
        for key in ("aero_emissions", "aero_background"):
            scenario_ctor_arg[key][2]["dist"] = [[AERO_MODE_CTOR_CAMP]]

        scenario = ppmc.Scenario(gas_data, aero_data, scenario_ctor_arg)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        scenario.init_env_state(env_state, 0)

        # act
        getattr(ppmc, run_part_variant)(
            scenario,
            env_state,
            aero_data,
            ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL, camp_core),
            gas_data,
            gas_state,
            ppmc.RunPartOpt(
                {
                    **RUN_PART_OPT_CTOR_ARG_SIMULATION,
                    "output_prefix": str(filename),
                    "do_camp_chem": True,
                    "t_output": 60,
                }
            ),
            camp_core,
            ppmc.Photolysis(camp_core),
            *variant_args,
        )

        # assert
        assert len(aero_data) == 5

    @staticmethod
    @pytest.mark.parametrize("scheme", ["ABIFM", "singular", "const"])
    def test_run_part_do_freezing(common_args, tmp_path, scheme):
        filename = tmp_path / "test"
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_HIGH_RH)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FREEZING)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_FREEZING)
        aero_state = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
        scenario_ctor_arg = copy.deepcopy(SCENARIO_CTOR_ARG_MINIMAL)
        scenario_ctor_arg["temp_profile"][0]["time"] = [0, 900]
        scenario_ctor_arg["temp_profile"][1]["temp"] = [263.0, 243.0]
        args = list(common_args)
        args[0] = ppmc.Scenario(args[4], aero_data, scenario_ctor_arg)
        args[1] = env_state
        args[0].init_env_state(env_state, 0.0)
        args[2] = aero_data
        args[3] = aero_state
        if scheme == "const":
            args[6] = ppmc.RunPartOpt(
                {
                    **RUN_PART_OPT_CTOR_ARG_SIMULATION,
                    "output_prefix": str(filename),
                    "t_max": 3600,
                    "do_immersion_freezing": True,
                    "immersion_freezing_scheme": scheme,
                    "freezing_rate": -1e-4,
                }
            )
        else:
            args[6] = ppmc.RunPartOpt(
                {
                    **RUN_PART_OPT_CTOR_ARG_SIMULATION,
                    "output_prefix": str(filename),
                    "t_max": 3600,
                    "do_immersion_freezing": True,
                    "immersion_freezing_scheme": scheme,
                }
            )
        aero_state.dist_sample(aero_dist, 1.0, 0.0, False, False)
        ppmc.run_part(*args)

        assert aero_state.frozen_fraction > 0.0
