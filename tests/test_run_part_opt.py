####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc

import pytest

import PyPartMC as ppmc
from PyPartMC import si

RUN_PART_OPT_CTOR_ARG_MINIMAL = {
    "do_coagulation": False,
    "do_parallel": False,
    "do_camp_chem": False,
    "t_max": 0,
    "del_t": 1 * si.s,
    "t_output": 0,
    "t_progress": 0,
    "allow_halving": False,
    "allow_doubling": False,
}


class TestRunPartOpt:
    @staticmethod
    @pytest.mark.parametrize("ctor_arg", (RUN_PART_OPT_CTOR_ARG_MINIMAL,))
    def test_ctor(ctor_arg):
        # arrange
        pass

        # act
        run_part_opt = ppmc.RunPartOpt(ctor_arg)

        # assert
        assert run_part_opt is not None

    @staticmethod
    def test_dtor():
        # arrange
        # pylint: disable=unused-variable
        run_part_opt = ppmc.RunPartOpt(RUN_PART_OPT_CTOR_ARG_MINIMAL)
        gc.collect()

        # act
        run_part_opt = None
        gc.collect()

        # assert
        pass
