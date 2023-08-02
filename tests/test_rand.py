####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import pytest

import PyPartMC as ppmc


@pytest.mark.order(-1)
@pytest.mark.parametrize(
    "seeds",
    (
        (44, 44),
        pytest.param((44, 45), marks=pytest.mark.xfail(strict=True)),
    ),
)
def test_set_rand_seed(seeds):
    # arrange
    rand_arg = (0, 1)
    values = []

    # act
    for seed in seeds:
        ppmc.rand_init(seed)
        values.append(ppmc.rand_normal(*rand_arg))

    # assert
    for value in values[1:]:
        assert value == values[0]
