####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import gc
import PyPartMC as ppmc


class TestAeroData:
    @staticmethod
    def test_ctor():
        # arrange

        # act
        sut = ppmc.AeroData()

        # assert
        assert sut is not None

    @staticmethod
    def test_dtor():
        # arrange
        sut = ppmc.AeroData()  # pylint: disable=unused-variable

        # act
        sut = None
        gc.collect()

        # assert
        pass
