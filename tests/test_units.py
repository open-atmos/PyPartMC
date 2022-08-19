import PyPartMC as ppmc

# pylint: disable=no-member


class TestUnits:
    @staticmethod
    def test_length():
        assert ppmc.si.m == 1
        assert ppmc.si.cm == 0.01
        assert ppmc.si.um == 1e-6

    @staticmethod
    def test_temperatur():
        assert ppmc.si.K == 1

    @staticmethod
    def test_time():
        assert ppmc.si.s == 1

    @staticmethod
    def test_pressure():
        assert ppmc.si.Pa == 1
        assert ppmc.si.hPa == 100

    @staticmethod
    def test_amount():
        assert ppmc.si.mol == 1

    @staticmethod
    def test_mass():
        assert ppmc.si.kg == 1
        assert ppmc.si.g == 1e-3
