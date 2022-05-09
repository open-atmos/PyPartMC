import pytest
import numpy as np
import PyPartMC as ppmc

class TestGasData:
  @staticmethod
  def test_ctor():
    # arrange

    # act
    sut = ppmc.GasData()

    # assert
    assert sut is not None

  @staticmethod
  def test_dtor():
    # arrange
    sut = ppmc.GasData()

    # act
    sut = None

    # assert
    assert sut is None

