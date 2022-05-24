####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import PyPartMC as ppmc
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL
from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_state import AERO_STATE_CTOR_ARG_MINIMAL

# pylint: disable=unused-variable


class TestCondense:
    @staticmethod
    def test_equilib_particles():
        # arrange
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_state = ppmc.AeroState(AERO_STATE_CTOR_ARG_MINIMAL, aero_data)

        # act
        # ppmc.condense_equilib_particles(env_state, aero_data, aero_state)

        # assert
        pass  # TODO

    @staticmethod
    def test_todo():
        pass
