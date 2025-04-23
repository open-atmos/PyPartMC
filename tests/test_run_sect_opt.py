####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2025 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc

import pytest

import PyPartMC as ppmc
from PyPartMC import si

from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL

RUN_SECT_OPT_CTOR_ARG_MINIMAL = {
    "output_prefix": "tests/test",
    "do_coagulation": False,
    "t_max": 0,
    "del_t": 1 * si.s,
}

RUN_SECT_OPT_CTOR_ARG_SIMULATION = {
    "output_prefix": "tests/test",
    "do_coagulation": True,
    "coag_kernel": "brown",
    "t_max": 86400.0,
    "del_t": 60 * si.s,
    "t_output": 3600.0,
    "t_progress": 3600.0,
}


class TestRunSectOpt:
    @staticmethod
    @pytest.mark.parametrize(
        "ctor_arg", (RUN_SECT_OPT_CTOR_ARG_MINIMAL, RUN_SECT_OPT_CTOR_ARG_SIMULATION)
    )
    def test_ctor(ctor_arg):
        # arrange
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        # act
        run_sect_opt = ppmc.RunSectOpt(ctor_arg, env_state)

        # assert
        assert run_sect_opt is not None

    @staticmethod
    def test_dtor():
        # arrange
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        # pylint: disable=unused-variable
        run_sect_opt = ppmc.RunSectOpt(RUN_SECT_OPT_CTOR_ARG_MINIMAL, env_state)
        gc.collect()

        # act
        run_sect_opt = None
        gc.collect()

        # assert
        pass

    @staticmethod
    def test_get_t_max():
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        run_sect_opt = ppmc.RunSectOpt(RUN_SECT_OPT_CTOR_ARG_MINIMAL, env_state)

        # act
        t_max = run_sect_opt.t_max

        # assert
        assert t_max == RUN_SECT_OPT_CTOR_ARG_MINIMAL["t_max"]

    @staticmethod
    def test_aero_del_t():
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        run_sect_opt = ppmc.RunSectOpt(RUN_SECT_OPT_CTOR_ARG_MINIMAL, env_state)

        # act
        del_t = run_sect_opt.del_t

        # assert
        assert del_t == RUN_SECT_OPT_CTOR_ARG_MINIMAL["del_t"]
