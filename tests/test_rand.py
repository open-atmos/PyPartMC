####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import PyPartMC as ppmc


@staticmethod
def test_set_rand_seed():
    # arrange
    init_arg = (1, 0)
    rand_arg = (0, 1)
    
    # act
    ppmc.rand_init(*init_arg)
    val_a = ppmc.rand_normal(*rand_arg)
    ppmc.rand_init(*init_arg)
    val_b = ppmc.rand_normal(*rand_arg)

    # assert
    assert val_a == val_b
