####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import PyPartMC as ppmc
import pytest
from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL

class TestAeroParticle:
    @staticmethod
    @pytest.mark.parametrize("volumes", (
        [0],
        pytest.param([],marks=pytest.mark.xfail(strict=True))
    ))
    def test_ctor(volumes):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act
        sut = ppmc.AeroParticle(aero_data, volumes)

        # assert
        assert sut is not None

    @staticmethod
    def test_volumes():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        volumes = [123]

        # act
        sut = ppmc.AeroParticle(aero_data, volumes)

        # assert
        assert sut.volumes == volumes
