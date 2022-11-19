####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc

import pytest

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_mode import AERO_MODE_CTOR_LOG_NORMAL
from .test_gas_data import GAS_DATA_CTOR_ARG_MINIMAL


@pytest.mark.parametrize(
    "sut",
    (
        pytest.param(ppmc.GasData(("SO2",)), id="GasData"),
        pytest.param(ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL), id="AeroData"),
        pytest.param(
            ppmc.GasState(ppmc.GasData(GAS_DATA_CTOR_ARG_MINIMAL)), id="GasState"
        ),
        pytest.param(
            ppmc.AeroParticle(ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL), [0]),
            id="AeroParticle",
        ),
        pytest.param(ppmc.Photolysis(), id="Photolysis"),
        pytest.param(ppmc.CampCore(), id="CampCore"),
        pytest.param(
            ppmc.AeroMode(
                ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL), AERO_MODE_CTOR_LOG_NORMAL
            ),
            id="AeroMode",
        ),
        pytest.param(
            ppmc.AeroState(1, ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)), id="AeroState"
        ),
    ),
)
def test_dtors(sut):  # pylint: disable=unused-argument
    # arrange
    gc.collect()

    # act
    sut = None
    gc.collect()

    # assert
    pass
