####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import sys
import pytest
import PyPartMC as ppmc
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL
from .test_run_part_opt import RUN_PART_OPT_CTOR_ARG_MINIMAL 


class TestRunPart:
    @staticmethod
    @pytest.mark.skipif(sys.platform != 'linux', reason="TODO #63")
    @pytest.mark.xfail(strict=True)
    def test_args():
        # arrange
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        aero_data = ppmc.AeroData()
        aero_state = ppmc.AeroState()
        gas_data  = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
        run_part_opt = ppmc.RunPartOpt(RUN_PART_OPT_CTOR_ARG_MINIMAL)

        # act
        ppmc.run_part(scenario, env_state, aero_data, aero_state, gas_data, run_part_opt)
        #print(scenario, env_state, aero_data, aero_state, gas_data, run_part_opt)

        # assert

    @staticmethod
    def test_todo():
        pass
