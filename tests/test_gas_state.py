####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import pytest
import json
import numpy as np
import PyPartMC as ppmc

class TestGasState:
    @staticmethod
    def test_ctor():
        # arrange

        # act
        sut = ppmc.GasState()

        # assert
        assert sut is not None
        assert isinstance(sut, ppmc.GasState)

    @staticmethod
    def test_dtor():
        # arrange
        sut = ppmc.GasState()

        # act
        sut = None

        # assert
        assert sut is None

    @staticmethod
    def test_len():
        # arrange
        sut = ppmc.GasState()

        # act
        size = len(sut)

        # assert
        assert isinstance(size, int)
        assert size == 33  # TODO

    @staticmethod
    @pytest.mark.parametrize("idx", (-1, 100))
    def test_get_item_out_of_range(idx):
        # arrange
        sut = ppmc.GasState()

        # act
        try:
            value = sut[idx]
        except IndexError:
            return

        # assert
        assert False

    @staticmethod
    def test_get_item_valid():
        # arrange
        sut = ppmc.GasState()

        # act
        value = sut[0]  # TODO

        # assert
        assert isinstance(value, float)
        #assert value == 44  # TODO

    @staticmethod
    def test_get_items():
        # arrange
        sut = ppmc.GasState()

        # act
        # values = sut[:]  TODO

        # assert
        # assert isinstance(values, np.ndarray)
        # assert len(sut) == len(values)

    @staticmethod
    def test_set_item():
        # arrange
        sut = ppmc.GasState()  # TODO
        idx = 1
        val = 1234

        # act
        # sut[idx] = val  TODO

        # assert
        # assert sut[idx] == value  TODO

    # TODO: set_items

    @staticmethod
    def test_from_json():
        # arrange
        data = {'aqq': (3,4,5)}
        json_str = json.dumps(data)

        # act
        ppmc.GasState(json_str)

        # assert
        pass
    
    @staticmethod
    def test_to_json():
        # arrange
        data_in = {'':''}
        sut = ppmc.GasState(json.dumps(data_in))

        # act
        data_out = str(sut)

        # assert
        # assert data_in == data_out TODO

