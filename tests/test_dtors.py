####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import gc
import pytest
import PyPartMC as ppmc
from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL


@pytest.mark.parametrize('sut', (
    pytest.param(ppmc.GasData(("SO2",)), id='GasData'),
    pytest.param(ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL), id='AeroData'),
    pytest.param(ppmc.GasState(), id='GasState')
))
def test_dtors(sut):  # pylint: disable=unused-argument
    # arrange
    gc.collect()

    # act
    sut = None
    gc.collect()

    # assert
    pass

