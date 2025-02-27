import copy

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_mode import AERO_MODE_CTOR_LOG_NORMAL, AERO_MODE_CTOR_SAMPLED

class TestInputGuard:
    @staticmethod
    def test_unused_parameter(capfd):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        mode = copy.deepcopy(AERO_MODE_CTOR_LOG_NORMAL)
        mode["test_mode"]["not_used"] = 0

        # act
        ppmc.AeroDist(aero_data, [mode])
        captured = capfd.readouterr()

        # assert
        assert captured.err == "WARNING: \"test_mode/not_used\" parameter remains unused.\n"

    @staticmethod
    def test_nested_unsued_parameter(capfd):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        mode = copy.deepcopy(AERO_MODE_CTOR_SAMPLED)
        mode["test_mode"]["size_dist2"] = [{"diam" : [1]}, {"num_conc" : [100]}]

        # act
        ppmc.AeroDist(aero_data, [mode])
        captured = capfd.readouterr()

        # assert
        assert captured.err == "WARNING: \"test_mode/size_dist2/diam\" parameter remains unused.\n"

    @staticmethod
    def test_same_name_unused_parameter(capfd):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        modes = {}
        modes["log_normal_mode"] = copy.deepcopy(AERO_MODE_CTOR_LOG_NORMAL["test_mode"])
        modes["sampled_mode"] = copy.deepcopy(AERO_MODE_CTOR_SAMPLED["test_mode"])
        modes["sampled_mode"]["geom_mean_diam"] = 10

        # act
        ppmc.AeroDist(aero_data, [modes])
        captured = capfd.readouterr()

        # assert
        assert captured.err == "WARNING: \"sampled_mode/geom_mean_diam\" parameter remains unused.\n"