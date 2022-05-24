####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

# TODO
# pylint: disable=unused-variable

import pytest
#import numpy as np
import PyPartMC as ppmc

class TestGasState:
    @staticmethod
    def test_ctor_error():
        # arrange
        pass

        # act
        #try:
        #    sut = ppmc.GasState({'':''})
        #except ValueError as err:
        #    assert "gas_mixing_ratio" in str(err)
        #    return

        # assert
        #assert False

    @staticmethod
    def test_ctor_emtpy():
        # arrange
        pass

        # act
        sut = ppmc.GasState()

        # assert
        isinstance(sut, ppmc.GasState)

    @staticmethod
    def test_ctor_valid():
        # arrange
        pass

        # act
        # TODO sut = ppmc.GasState({"gas_mixing_ratio":''})

        # assert
        # TODO assert isinstance(sut, ppmc.GasState)

    @staticmethod
    def test_len_empty():
        # arrange
        pass
        #sut = ppmc.GasState()

        # act
        #size = len(sut)

        # assert
        #assert isinstance(size, int)
        #assert size == 0  # TODO: test non-empty len

    @staticmethod
    @pytest.mark.parametrize("idx", (-1, 100))
    def test_get_item_out_of_range(idx):
        # arrange
        pass
        #sut = ppmc.GasState()

        # act
        #try:
        #    value = sut[idx]
        #except IndexError:
        #    return

        # assert
        #assert False

    @staticmethod
    def test_get_item_valid():
        pass
        # arrange
        #sut = ppmc.GasState({'gas_mixing_ratio': (44,)})

        # act
        #value = sut[0]

        # assert
        # assert isinstance(value, float)
        # assert value == 44  # TODO

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
    def test_to_json():
        # arrange
        data_in = {} # TODO
        sut = ppmc.GasState(data_in)

        # act
        data_out = str(sut)
        print(data_out)

        # assert
        #assert data_in == data_out  TODO

