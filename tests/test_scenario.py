####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import pytest
import PyPartMC as ppmc


class TestScenario:
    @staticmethod
    #@pytest.mark.xfail(strict=True)
    def test_ctor():
        # arrange
        aero_data = ppmc.AeroData()
        gas_data = ppmc.GasData(("SO2", "NO2"))

        # act
        sut = ppmc.Scenario(gas_data, aero_data, {
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
        })

        # assert
        assert sut is not None

    @staticmethod
    #@pytest.mark.xfail(strict=True)
    def test_dtor():
        # arrange
        pass
        #sut = ppmc.Scenario()

        # act
        #sut = None

        # assert
        #assert sut is None

