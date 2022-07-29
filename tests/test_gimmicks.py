import json
import PyPartMC as ppmc
import pytest

from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL
from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_scenario import SCENARIO_CTOR_ARG_MINIMAL

FAKE_JSON = {
    'temp_profile': [{'time': []}, {'temp': []}],
    'pressure_profile': [{'time': []}, {'pressure': []}],
    'height_profile': [{'time': []}, {'height': []}],
    'gas_emissions': [{'time': [0]}, {'rate': [0]}, {'SO2': [0]}],
    'gas_background': [{'time': [0]}, {'rate': [0]}, {'SO2': [0]}],
    'aero_emissions': [{'time': [0]}, {'rate': [0]}, {'dist': [{}]}],
    'aero_background': [{'time': [0]}, {'rate': [0]}, {'dist': [{}]}],
    'loss_function': 'none',

    # this field is fake
    'humidity': 'none'
}

class TestJSONInput:
    @staticmethod
    def test_json():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        gas_data = ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)
        
        # act & assert
        with pytest.raises(RuntimeError):
            fake_scenario = ppmc.Scenario(gas_data, aero_data, FAKE_JSON)

