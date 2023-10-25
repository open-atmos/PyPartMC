import pytest

import PyPartMC as ppmc


@pytest.fixture(autouse=True)
def rand_init():
    ppmc.rand_init(44)
