import pytest
import gc
import PyPartMC as ppmc


@pytest.mark.parametrize('instantiate', (
    pytest.param(lambda : ppmc.GasData(("SO2",)), id='GasData'),
    pytest.param(lambda : ppmc.AeroData(), id='AeroData'),
    pytest.param(lambda : ppmc.GasState(), id='GasState')
))
def test_dtors(instantiate):
    # arrange
    sut = instantiate()  # pylint: disable=unused-variable
    gc.collect()

    # act
    sut = None
    gc.collect()

    # assert
    pass

