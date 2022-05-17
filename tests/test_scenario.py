####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import pytest
import PyPartMC as ppmc


class TestScenario:
    @staticmethod
    @pytest.mark.xfail(strict=True)
    def test_ctor():
        # arrange

        # act
        sut = ppmc.Scenario({'temp_profile': '123'})

        # assert
        assert sut is not None

    @staticmethod
    @pytest.mark.xfail(strict=True)
    def test_dtor():
        # arrange
        sut = ppmc.Scenario()

        # act
        sut = None

        # assert
        assert sut is None

