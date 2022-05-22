import gc
import pytest
import PyPartMC as ppmc


@pytest.mark.parametrize('sut', (
    pytest.param(ppmc.GasData(("SO2",)), id='GasData'),
    pytest.param(ppmc.AeroData(), id='AeroData'),
    pytest.param(ppmc.GasState(), id='GasState')
))
def test_dtors(sut):  # pytest: disable=unused-argument
    # arrange
    gc.collect()

    # act
    sut = None
    gc.collect()

    # assert
    pass

