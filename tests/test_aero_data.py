####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import PyPartMC as ppmc
from PyPartMC import si

AERO_DATA_CTOR_ARG_MINIMAL = (
    {"H20": [1000 * si.kg / si.m**3, 1, 18e-3 * si.kg / si.mol, 0]},
)


class TestAeroData:
    @staticmethod
    def test_ctor():
        # arrange

        # act
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # assert
        assert sut is not None

    @staticmethod
    def test_std():
        pass  # TODO
