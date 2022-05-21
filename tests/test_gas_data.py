####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import gc
import PyPartMC as ppmc


class TestGasData:
    @staticmethod
    def test_ctor():
        # arrange
        pass

        # act
        sut = ppmc.GasData(("SO2",))

        # assert
        assert sut is not None

    @staticmethod
    def test_dtor():
        # arrange
        sut = ppmc.GasData(("SO2",))

        # act
        sut = None
        gc.collect()

        # assert
        pass

