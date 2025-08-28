import os
from contextlib import contextmanager
from pathlib import Path

import pytest

import PyPartMC as ppmc

CAMP_INPUT_PATH = (
    Path(ppmc.__file__).parent.parent.parent / "gitmodules" / "partmc" / "test" / "camp"
)


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
    @pytest.mark.skipif(
        "site-packages" in ppmc.__file__, reason="Skipped for wheel install"
    )
    def test_ctor_with_args_assuming_installed_in_editable_mode_from_checkout():
        # arrange
        assert CAMP_INPUT_PATH.exists()

        # act
        with chdir(CAMP_INPUT_PATH):
            _ = ppmc.CampCore("config.json")
