####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc
import platform

import numpy as np
import pytest

import PyPartMC as ppmc

from .common import ENV_STATE_CTOR_ARG_MINIMAL
from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL

GAS_DATA_MINIMAL = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)


GAS_STATE_MINIMAL = ({"SO2": [0.1]},)


class TestGasState:
    @staticmethod
    def test_ctor_valid():
        # act
        sut = ppmc.GasState(GAS_DATA_MINIMAL)

        # assert
        assert isinstance(sut, ppmc.GasState)

    @staticmethod
    @pytest.mark.parametrize("idx", (-1, 100))
    @pytest.mark.skipif(platform.machine() == "arm64", reason="TODO #348")
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

    @staticmethod
    def test_scale():
        # arrange
        gas_data = ppmc.GasData(
            (
                "SO2",
                "CO",
            )
        )
        sut = ppmc.GasState(gas_data)
        gas_state_init_values = ({"SO2": [0.1]}, {"CO": [0.5]})
        sut.mix_rats = gas_state_init_values

        # act
        alpha = 5.0
        sut.scale(alpha)

        # assert
        idx_set = []
        for item in gas_state_init_values:
            keys = item.keys()
            assert len(keys) == 1
            key = tuple(keys)[0]
            val = tuple(item.values())[0][0]
            idx_set.append(gas_data.spec_by_name(key))
            assert sut[gas_data.spec_by_name(key)] == val * alpha

    @staticmethod
    def test_molar_conc_to_ppb():
        # arrange
        gas_data = GAS_DATA_MINIMAL
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        scenario = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        scenario.init_env_state(env_state, 0.0)
        sut = ppmc.GasState(GAS_DATA_MINIMAL)
        sut.mix_rats = GAS_STATE_MINIMAL

        # act
        sut.molar_conc_to_ppb(env_state)

        # assert
        assert sut.mix_rats[0] > GAS_STATE_MINIMAL[0][gas_data.species[0]][0]

    @staticmethod
    def test_add():
        gas_data = GAS_DATA_MINIMAL
        A = ppmc.GasState(GAS_DATA_MINIMAL)
        A.mix_rats = GAS_STATE_MINIMAL
        B = ppmc.GasState(GAS_DATA_MINIMAL)
        B.mix_rats = GAS_STATE_MINIMAL
        tot_mix_rats = np.add(A.mix_rats, B.mix_rats)

        # act
        A.add(B)

        assert A.mix_rats == tot_mix_rats

    @staticmethod
    def test_set_mix_rats_from_json():
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
        gas_state_init_values = ({"SO2": [0.1]}, {"CO": [0.5]})
        sut.mix_rats = gas_state_init_values

        # assert
        idx_set = []
        for item in gas_state_init_values:
            keys = item.keys()
            assert len(keys) == 1
            key = tuple(keys)[0]
            val = tuple(item.values())[0][0]
            idx_set.append(gas_data.spec_by_name(key))
            assert sut[gas_data.spec_by_name(key)] == val
        for i_spec in range(gas_data.n_spec):
            if not i_spec in idx_set:
                assert sut[i_spec] == 0

    @staticmethod
    @pytest.mark.skipif(platform.machine() == "arm64", reason="TODO #348")
    def test_set_mix_rats_empty():
        # arrange
        gas_data = ppmc.GasData(("SO2",))
        sut = ppmc.GasState(gas_data)

        # act
        with pytest.raises(RuntimeError) as excinfo:
            sut.mix_rats = ()

        # assert
        assert str(excinfo.value) == "Non-empty sequence of mixing ratios expected"
