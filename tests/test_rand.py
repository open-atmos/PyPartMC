####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import pytest

import PyPartMC as ppmc


@pytest.mark.parametrize(
    "init_args",
    (
        ((0, 1), (0, 1)),
        pytest.param(((0, 1), (0, 44)), marks=pytest.mark.xfail(strict=True)),
    ),
)
def test_set_rand_seed(init_args):
    # arrange
    rand_arg = (0, 1)
    values = []

    # act
    for init_arg in init_args:
        ppmc.rand_init(*init_arg)
        values.append(ppmc.rand_normal(*rand_arg))

    # assert
    for value in values[1:]:
        assert value == values[0]
