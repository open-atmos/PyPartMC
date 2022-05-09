import pytest
import numpy as np
import PyPartMC as ppmc

class TestAeroData:
  @staticmethod
  def test_ctor():
    # arrange

    # act
    sut = ppmc.AeroData()

    # assert
    assert sut is not None

  @staticmethod
  def test_dtor():
    # arrange
    sut = ppmc.AeroData()

    # act
    sut = None

    # assert
    assert sut is None

