####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

# TODO #123
# pylint: disable=unused-variable

import pytest

# import numpy as np
import PyPartMC as ppmc

from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL

GAS_DATA_MINIMAL = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)


class TestGasState:
    @staticmethod
    def test_ctor_error():
        # arrange
        pass

        # act
        # try:
        #    sut = ppmc.GasState({'':''})
        # except ValueError as err:
        #    assert "gas_mixing_ratio" in str(err)
        #    return

        # assert
        # assert False

    @staticmethod
    def test_ctor_valid():
        # arrange
        sut = ppmc.GasState(GAS_DATA_MINIMAL)

        # assert
        assert isinstance(sut, ppmc.GasState)

    @staticmethod
    @pytest.mark.parametrize("idx", (-1, 100))
    def test_get_item_out_of_range(idx):
        # arrange
        sut = ppmc.GasState(GAS_DATA_MINIMAL)

        # act
        try:
            value = sut[idx]
        except IndexError:
            return

        assert False

    @staticmethod
    def test_get_item_valid():
        # arrange
        sut = ppmc.GasState(GAS_DATA_MINIMAL)
        sut[0] = 44
        # act
        value = sut[0]

        # assert
        assert isinstance(value, float)
        assert value == 44

    @staticmethod
    def test_get_items():
        # arrange
        gas_data = ppmc.GasData(
            (
                "SO2",
                "NO2",
                "NO",
                "CO",
            )
        )
        sut = ppmc.GasState(gas_data)

        # act
        values = sut.mix_rats

        # assert
        assert isinstance(values, list)
        assert len(sut) == len(values)

    @staticmethod
    def test_set_item():
        # arrange
        sut = ppmc.GasState(GAS_DATA_MINIMAL)
        idx = 0
        val = 1234

        # act
        sut[idx] = val

        # assert
        assert sut[idx] == val

    @staticmethod
    def test_get_mix_rats():

        gas_data = GAS_DATA_MINIMAL
        sut = ppmc.GasState(gas_data)

        assert len(sut.mix_rats) == len(sut)


#    @staticmethod
#    def test_to_json():
#        # arrange
#        data_in = {}  # TODO #123
#        sut = ppmc.GasState(data_in)
#
#        # act
#        data_out = str(sut)
#        print(data_out)
#
#        # assert
#        # assert data_in == data_out  TODO #123
