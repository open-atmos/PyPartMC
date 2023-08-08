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
    "output_prefix": "tests/test",
    "do_coagulation": False,
    "t_max": 0,
    "del_t": 1 * si.s,
}

RUN_PART_OPT_CTOR_ARG_SIMULATION = {
    "output_prefix": "tests/test",
    "do_coagulation": True,
    "coag_kernel": "brown",
    "do_parallel": False,
    "do_nucleation": False,
    "do_mosaic": False,
    "do_optical": False,
    "do_condensation": False,
    "do_camp_chem": False,
    "t_max": 86400.0,
    "del_t": 60 * si.s,
    "t_output": 3600.0,
    "t_progress": 0.0,
    "rand_init": 0,
    "allow_halving": False,
    "allow_doubling": False,
}


class TestRunPartOpt:
    @staticmethod
    @pytest.mark.parametrize(
        "ctor_arg", (RUN_PART_OPT_CTOR_ARG_MINIMAL, RUN_PART_OPT_CTOR_ARG_SIMULATION)
    )
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

    @staticmethod
    def test_get_t_max():
        # arrange
        run_part_opt = ppmc.RunPartOpt(RUN_PART_OPT_CTOR_ARG_MINIMAL)

        # act
        t_max = run_part_opt.t_max

        # assert
        assert t_max == RUN_PART_OPT_CTOR_ARG_MINIMAL["t_max"]

    @staticmethod
    def test_aero_del_t():
        run_part_opt = ppmc.RunPartOpt(RUN_PART_OPT_CTOR_ARG_MINIMAL)

        # act
        del_t = run_part_opt.del_t

        # assert
        assert del_t == RUN_PART_OPT_CTOR_ARG_MINIMAL["del_t"]
