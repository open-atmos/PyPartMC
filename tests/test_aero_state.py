####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import PyPartMC as ppmc
from PyPartMC import si

AERO_STATE_CTOR_ARG_MINIMAL = 44


class TestAeroState:
    @staticmethod
    def test_ctor():
        # arrange

        # act
        sut = ppmc.AeroState(AERO_STATE_CTOR_ARG_MINIMAL)

        # assert
        assert sut is not None

    @staticmethod
    def test_todo():
        pass  # TODO
