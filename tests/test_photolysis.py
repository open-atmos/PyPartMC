import PyPartMC as ppmc


class TestPhotolysis:
    @staticmethod
    def test_ctor_without_camp():
        sut = ppmc.Photolysis()

    @staticmethod
    def test_ctor_with_camp():
        sut = ppmc.Photolysis(ppmc.CampCore())
