import pytest
import PyPartMC as ppmc

class TestRunPartOptT:
    @staticmethod
    def test_t_max():
        run_part_opt = ppmc.run_part_opt_t()
