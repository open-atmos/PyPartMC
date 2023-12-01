####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import pytest

import PyPartMC as ppmc


def test_version():
    assert 3 < len(ppmc.__version__) < 20


@pytest.mark.parametrize(
    "pkg",
    (
        "PartMC",
        "pybind11",
        "CAMP",
        "SUNDIALS",
    ),
)
def test_versions_of_build_time_dependencies(pkg):
    sut = ppmc.__versions_of_build_time_dependencies__
    assert 3 < len(sut[pkg]) < 20
    assert '"' not in sut[pkg]


def test_versions_of_build_time_dependencies_printable():
    print(ppmc.__versions_of_build_time_dependencies__)
