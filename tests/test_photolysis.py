import PyPartMC as ppmc


class TestPhotolysis:
    @staticmethod
    def test_ctor_without_camp():
        _ = ppmc.Photolysis()

    @staticmethod
    def test_ctor_with_camp():
        _ = ppmc.Photolysis(ppmc.CampCore())
