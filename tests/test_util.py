import PyPartMC as ppmc


class TestUtil:
    @staticmethod
    def test_pow2_above():
        # arrange
        n = 13

        # act
        pow2 = ppmc.util.pow2_above(n)

        # assert
        assert pow2 == 16
