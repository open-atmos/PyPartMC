####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_state import AERO_STATE_CTOR_ARG_MINIMAL
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_run_part_opt import RUN_PART_OPT_CTOR_ARG_MINIMAL
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL


class TestRunPart:
    @staticmethod
    def test_args():
        # pylint: disable=unused-variable
        # arrange
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_state = ppmc.AeroState(AERO_STATE_CTOR_ARG_MINIMAL, aero_data)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        gas_state = ppmc.GasState(gas_data)
        scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
        run_part_opt = ppmc.RunPartOpt(RUN_PART_OPT_CTOR_ARG_MINIMAL)
        camp_core = ppmc.CampCore()
        photolysis = ppmc.Photolysis()

        # act
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

        # assert
        pass

    @staticmethod
    def test_todo():
        pass
