####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import PyPartMC as ppmc


class TestUtil:
    @staticmethod
    def test_pow2_above():
        # arrange
        arg = 13

        # act
        pow2 = ppmc.pow2_above(arg)

        # assert
        assert pow2 == 16

    @staticmethod
    def test_deg2rad():
        pass  # TODO

