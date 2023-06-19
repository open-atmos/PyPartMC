####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import json

import pytest

import PyPartMC as ppmc

GAS_DATA_CTOR_ARG_MINIMAL = ("SO2",)


class TestGasData:
    @staticmethod
    def test_ctor():
        # arrange
        pass

        # act
        sut = ppmc.GasData(("SO2",))

        # assert
        assert sut is not None

    @staticmethod
    def test_len():
        # arrange
        data = ("X", "Y", "Z")
        sut = ppmc.GasData(data)

        # act
        size = len(sut)

        # assert
        assert size == len(data)

    @staticmethod
    def test_str():
        # arrange
        data = ("A", "B", "C")
        sut = ppmc.GasData(data)

        # act
        string = str(sut)

        # assert
        assert string == json.dumps(data, separators=(",", ":"))

    @staticmethod
    @pytest.mark.parametrize(
        "ctor_arg", (GAS_DATA_CTOR_ARG_MINIMAL, ("SO2", "NO2"), ("A", "B", "C"))
    )
    def test_spec_by_name(ctor_arg):
        # arrange
        sut = ppmc.GasData(ctor_arg)

        # act
        indices = [sut.spec_by_name(name) for name in ctor_arg]

        # assert
        assert indices == list(range(len(ctor_arg)))
