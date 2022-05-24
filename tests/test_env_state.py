####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc
import pytest
import PyPartMC as ppmc

ENV_STATE_CTOR_ARG_MINIMAL = {
    'rel_humidity': 0.,
    'latitude': 0.,
    'longitude': 0.,
    'altitude': 0.,
    'start_time': 0.,
    'start_day': 0
}


class TestEnvState:
    @staticmethod
    @pytest.mark.parametrize('ctor_arg', (ENV_STATE_CTOR_ARG_MINIMAL,))
    def test_ctor(ctor_arg):
        # arrange
        pass

        # act
        sut = ppmc.EnvState(ctor_arg)

        # assert
        assert sut is not None

    @staticmethod
    def test_dtor():
        # arrange
        sut = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)  # pylint: disable=unused-variable
        gc.collect()

        # act
        sut = None
        gc.collect()

        # assert
        pass

