####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_mode import AERO_MODE_CTOR_LOG_NORMAL

AERO_DIST_CTOR_ARG_MINIMAL = [
    AERO_MODE_CTOR_LOG_NORMAL,
]


# pylint: disable=too-few-public-methods
class TestAeroDist:
    @staticmethod
    def test_ctor():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act
        sut = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)

        # assert
        assert sut is not None
