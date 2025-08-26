import os
from contextlib import contextmanager
from pathlib import Path

import PyPartMC as ppmc


@contextmanager
def chdir(path):
    prev_cwd = os.getcwd()
    try:
        os.chdir(path)
        yield
    finally:
        os.chdir(prev_cwd)


class TestCampCore:
    @staticmethod
    def test_ctor_no_arg():
        # arrange

        # act
        sut = ppmc.CampCore()

        # assert
        assert sut is not None

    @staticmethod
    def test_ctor_with_args_assuming_installed_in_editable_mode_from_checkout():
        # arrange
        config_path = (
            Path(ppmc.__file__).parent.parent.parent
            / "gitmodules"
            / "partmc"
            / "test"
            / "camp"
        )
        assert config_path.exists()

        # act
        with chdir(config_path):
            _ = ppmc.CampCore("config.json")
