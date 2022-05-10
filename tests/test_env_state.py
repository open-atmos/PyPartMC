####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import PyPartMC as ppmc

class TestEnvState:
    @staticmethod
    def test_ctor():
        # arrange

        # act
        sut = ppmc.EnvState()

        # assert
        assert sut is not None

    @staticmethod
    def test_dtor():
        # arrange
        sut = ppmc.EnvState()

        # act
        sut = None

        # assert
        assert sut is None

