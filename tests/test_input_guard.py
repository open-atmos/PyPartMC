import copy
import platform

import pytest

import PyPartMC as ppmc

from .common import ENV_STATE_CTOR_ARG_MINIMAL
from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_mode import AERO_MODE_CTOR_LOG_NORMAL, AERO_MODE_CTOR_SAMPLED
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_run_part_opt import RUN_PART_OPT_CTOR_ARG_MINIMAL
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL


@pytest.mark.skipif(platform.machine() == "arm64", reason="TODO #348")
class TestInputGuard:
    @staticmethod
    def test_aero_dist_unused_parameter():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        mode = copy.deepcopy(AERO_MODE_CTOR_LOG_NORMAL)
        mode["test_mode"]["not_used"] = 0

        # act
        with pytest.raises(RuntimeError) as exc_info:
            ppmc.AeroDist(aero_data, [mode])

        # assert
        assert (
            str(exc_info.value)
            == 'WARNING: "test_mode/not_used" parameter remains unused.'
        )

    @staticmethod
    def test_aero_mode_unused_parameter():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        mode = copy.deepcopy(AERO_MODE_CTOR_LOG_NORMAL)
        mode["test_mode"]["not_used"] = 0

        # act
        with pytest.raises(RuntimeError) as exc_info:
            ppmc.AeroMode(aero_data, mode)

        # assert
        assert (
            str(exc_info.value)
            == 'WARNING: "test_mode/not_used" parameter remains unused.'
        )

    @staticmethod
    def test_env_state_unused_parameter():
        # arrange
        args = copy.deepcopy(ENV_STATE_CTOR_ARG_MINIMAL)
        args["not_used"] = 0

        # act
        with pytest.raises(RuntimeError) as exc_info:
            ppmc.EnvState(args)

        # assert
        assert str(exc_info.value) == 'WARNING: "not_used" parameter remains unused.'

    @staticmethod
    def test_run_part_opt_unused_parameter():
        # arrange
        args = copy.deepcopy(RUN_PART_OPT_CTOR_ARG_MINIMAL)
        args["not_used"] = 0

        # act
        with pytest.raises(RuntimeError) as exc_info:
            ppmc.RunPartOpt(args)

        assert str(exc_info.value) == 'WARNING: "not_used" parameter remains unused.'

    @staticmethod
    def test_scenario_unused_parameter():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        args = copy.deepcopy(SCENARIO_CTOR_ARG_MINIMAL)
        args["not_used"] = 0

        # act
        with pytest.raises(RuntimeError) as exc_info:
            ppmc.Scenario(gas_data, aero_data, args)

        assert str(exc_info.value) == 'WARNING: "not_used" parameter remains unused.'

    @staticmethod
    def test_nested_unused_parameter():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        mode = copy.deepcopy(AERO_MODE_CTOR_SAMPLED)
        mode["test_mode"]["size_dist2"] = [{"diam": [1]}, {"num_conc": [100]}]

        # act
        with pytest.raises(RuntimeError) as exc_info:
            ppmc.AeroDist(aero_data, [mode])

        # assert
        assert (
            str(exc_info.value)
            == 'WARNING: "test_mode/size_dist2/diam" parameter remains unused.'
        )

    @staticmethod
    def test_same_name_unused_parameter():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        modes = {}
        modes["log_normal_mode"] = copy.deepcopy(AERO_MODE_CTOR_LOG_NORMAL["test_mode"])
        modes["sampled_mode"] = copy.deepcopy(AERO_MODE_CTOR_SAMPLED["test_mode"])
        modes["sampled_mode"]["geom_mean_diam"] = 10

        # act
        with pytest.raises(RuntimeError) as exc_info:
            ppmc.AeroDist(aero_data, [modes])

        # assert
        assert (
            str(exc_info.value)
            == 'WARNING: "sampled_mode/geom_mean_diam" parameter remains unused.'
        )
