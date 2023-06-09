####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import numpy as np

import PyPartMC as ppmc


class TestRand:
    @staticmethod
    def test_set_rand_seed():
        # arrange

        # act
        ppmc.rand_init(1, 0)
        val_a = ppmc.rand_normal(0, 1)
        ppmc.rand_init(1, 0)
        val_b = ppmc.rand_normal(0, 1)

        # assert
        assert val_a == val_b
