import pytest
import numpy as np
import PyPartMC as ppmc

class TestScenario:
  @staticmethod
  def test_ctor():
    # arrange

    # act
    sut = ppmc.Scenario()

    # assert
    assert sut is not None

  @staticmethod
  def test_dtor():
    # arrange
    sut = ppmc.Scenario()

    # act
    sut = None

    # assert
    assert sut is None

