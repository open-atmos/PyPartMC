import pytest
import numpy as np
import PyPartMC as ppmc

class TestGasState:
  @staticmethod
  def test_ctor():
    # arrange
    n = 44

    # act
    sut = ppmc.GasState(n)

    # assert
    assert sut is not None

  @staticmethod
  def test_dtor():
    # arrange
    sut = ppmc.GasState(666)

    # act
    sut = None

    # assert
    assert sut is None

  @staticmethod
  @pytest.mark.parametrize("val", (np.asarray((1,2,3.)),))
  def test_set_item(val):
    # arrange
    sut = ppmc.GasState(3)

    # act
    sut[:] = val

    # assert
    
  @staticmethod
  @pytest.mark.parametrize("val", (np.asarray((1,2,3.)),))
  def test_get_item(val):
    # arrange
    sut = ppmc.GasState(3)
    sut[:] = val

    # act
    actual = sut[:]

    # assert
    # TODO np.testing.assert_array_equal(val, actual)

