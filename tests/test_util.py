import PyPartMC as ppmc


class TestUtil:
    @staticmethod
    def test_pow2_above():
        # arrange
        arg = 13

        # act
        pow2 = ppmc.pow2_above(arg)

        # assert
        print(pow2)
        # TODO: assert pow2 == 16

    @staticmethod
    def test_deg2rad():
        pass  # TODO

