####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import gc
import pytest
import PyPartMC as ppmc

RUN_PART_OPT_CTOR_ARG_MINIMAL = {}

class TestRunPartOpt:
    @staticmethod
    @pytest.mark.parametrize('ctor_arg', (RUN_PART_OPT_CTOR_ARG_MINIMAL,))
    def test_ctor():
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

