####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc

import pytest

import PyPartMC as ppmc

from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL

GAS_DATA_MINIMAL = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)


class TestGasState:
    @staticmethod
    def test_ctor_valid():
        # act
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
            _ = sut[idx]
        except IndexError:
            return

        # assert
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
        gas_data = None
        gc.collect()

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
        # arrange
        gas_data = GAS_DATA_MINIMAL

        # act
        sut = ppmc.GasState(gas_data)

        # assert
        assert len(sut.mix_rats) == len(sut)
