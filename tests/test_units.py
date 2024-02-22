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
        assert ppmc.si.pK == 1e-12

    @staticmethod
    def test_time():
        assert ppmc.si.s == 1
        assert ppmc.si.us == 1e-6

    @staticmethod
    def test_pressure():
        assert ppmc.si.Pa == 1
        assert ppmc.si.hPa == 100

    @staticmethod
    def test_amount():
        assert ppmc.si.mol == 1
        assert ppmc.si.Tmol == 1e12

    @staticmethod
    def test_mass():
        assert ppmc.si.kg == 1
        assert ppmc.si.g == 1e-3

    @staticmethod
    def test_energy():
        assert ppmc.si.J == 1
        assert ppmc.si.MJ == 1e6

    @staticmethod
    def test_force():
        assert ppmc.si.N == 1
        assert ppmc.si.GN == 1e9

    @staticmethod
    def test_work():
        assert ppmc.si.W == 1
        assert ppmc.si.uW == 1e-6
