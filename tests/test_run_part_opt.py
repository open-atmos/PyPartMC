####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

import gc
import PyPartMC as ppmc


class TestRunPartOpt:
    @staticmethod
    def test_t_max():
        # arrange
        run_part_opt = ppmc.RunPartOpt()

        # act
        ##run_part_opt.t_max = 44.44

        # assert
        assert run_part_opt is not None
        ##assert run_part_opt.t_max == 44.44

    @staticmethod
    def test_t_output():
        # arrange
        run_part_opt = ppmc.RunPartOpt()

        # act
        ##run_part_opt.t_output = 66.6

        # assert
        assert run_part_opt is not None
        ##assert run_part_opt.t_output == 66.6

    @staticmethod
    def test_dtor():
        # arrange
        run_part_opt = ppmc.RunPartOpt()
        gc.collect()

        # act
        run_part_opt = None
        gc.collect()

        # assert
        pass

