####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import gc
import pytest
import PyPartMC as ppmc


GAS_DATA_CTOR_ARG_MINIMAL = ("X",)
SCENARIO_CTOR_ARG_MINIMAL = {
    'temp_profile': [{'time': []}, {'temp': []}],
    'pressure_profile': [{'time': []}, {'pressure': []}],
    'height_profile': [{'time': []}, {'height': []}],
    'gas_emissions': [{'time': [0]}, {'rate': [0]}, {'X': [0]}],
    'gas_background': [{'time': [0]}, {'rate': [0]}, {'X': [0]}],
    'aero_emissions': [{'time': [0]}, {'rate': [0]}, {'dist': [{}]}],
    'aero_background': [{'time': [0]}, {'rate': [0]}, {'dist': [{}]}],
    'loss_function': 'none'
}


class TestScenario:
    @staticmethod
    @pytest.mark.parametrize("params", (
        {
            "gas_data_ctor_arg": GAS_DATA_CTOR_ARG_MINIMAL,
            "scenario_ctor_arg": SCENARIO_CTOR_ARG_MINIMAL
        },
        {
            "gas_data_ctor_arg": ("SO2", "NO2"),
            "scenario_ctor_arg": {
                'temp_profile': [
                    {'time': [0,1,2, 3, 4]},
                    {'temp': [1,2,3]}
                ],
                'pressure_profile': [
                    {'time': [0,1,2, 3, 4]},
                    {'pressure': [1,2,3]}
                ],
                'height_profile': [
                    {'time': [0,1,2, 3, 4]},
                    {'height': [1,2,3]}
                ],
                'gas_emissions': [
                    {'time': [0, 1, 2]},
                    {'rate': [1, 1, 1]},
                    {'SO2':  [0, 0, 0]},
                    {'NO2':  [0, 0, 0]}
                ],
                'gas_background': [
                    {'time': [0, 1, 2]},
                    {'rate': [1, 1, 1]},
                    {'SO2':  [0, 0, 0]},
                    {'NO2':  [0, 0, 0]}
                ],
                'aero_emissions': [
                    {'time': [0]},
                    {'rate': [0]},
                    {'dist': [
                        {}
                    ]}
                ],
                'aero_background': [
                    {'time': [0]},
                    {'rate': [0]},
                    {'dist': [
                        {}
                    ]}
                ],
                'loss_function': 'none'
            },
        },
    ))
    def test_ctor(params):
        # arrange
        aero_data = ppmc.AeroData()
        gas_data = ppmc.GasData(params["gas_data_ctor_arg"])

        # act
        sut = ppmc.Scenario(gas_data, aero_data, params["scenario_ctor_arg"])

        # assert
        assert sut is not None

    @staticmethod
    def test_dtor():
        # arrange
        aero_data = ppmc.AeroData()
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        # pylint: disable=unused-variable
        sut = ppmc.Scenario(gas_data, aero_data, SCENARIO_CTOR_ARG_MINIMAL)

        # act
        sut = None
        gc.collect()

        # assert
        pass

