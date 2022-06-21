####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import PyPartMC as ppmc

# pylint: disable=too-few-public-methods
class TestAeroParticle:
    @staticmethod
    def test_ctor():
        # arrange

        # act
        sut = ppmc.AeroParticle()

        # assert
        assert sut is not None
