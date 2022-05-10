####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import PyPartMC as ppmc


class TestRunPart:
    @staticmethod
    def test_args():
        # arrange
        scenario = ppmc.Scenario()
        env_state = ppmc.EnvState()
        aero_data = ppmc.AeroData()
        aero_state = ppmc.AeroState()
        gas_data  = ppmc.GasData()
        run_part_opt = ppmc.RunPartOpt()

        # act
        #ppmc.run_part(scenario, env_state, aero_data, aero_state, gas_data, run_part_opt)
        print(scenario, env_state, aero_data, aero_state, gas_data, run_part_opt)

        # assert

    @staticmethod
    def test_todo():
        pass
