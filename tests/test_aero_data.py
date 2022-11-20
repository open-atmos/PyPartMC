####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import numpy as np
import pytest

import PyPartMC as ppmc
from PyPartMC import si

AERO_DATA_CTOR_ARG_MINIMAL = (
    {"H2O": [1000 * si.kg / si.m**3, 1, 18e-3 * si.kg / si.mol, 0]},
)

AERO_DATA_CTOR_ARG_FULL = (
    #         density  ions in soln (1) molecular weight    kappa (1)
    #         |                     |   |                   |
    {"SO4": [1800 * si.kg / si.m**3, 1, 96.0 * si.g / si.mol, 0.00]},
    {"NO3": [1800 * si.kg / si.m**3, 1, 62.0 * si.g / si.mol, 0.00]},
    {"Cl": [2200 * si.kg / si.m**3, 1, 35.5 * si.g / si.mol, 0.00]},
    {"NH4": [1800 * si.kg / si.m**3, 1, 18.0 * si.g / si.mol, 0.00]},
    {"MSA": [1800 * si.kg / si.m**3, 0, 95.0 * si.g / si.mol, 0.53]},
    {"ARO1": [1400 * si.kg / si.m**3, 0, 150.0 * si.g / si.mol, 0.10]},
    {"ARO2": [1400 * si.kg / si.m**3, 0, 150.0 * si.g / si.mol, 0.10]},
    {"ALK1": [1400 * si.kg / si.m**3, 0, 140.0 * si.g / si.mol, 0.10]},
    {"OLE1": [1400 * si.kg / si.m**3, 0, 140.0 * si.g / si.mol, 0.10]},
    {"API1": [1400 * si.kg / si.m**3, 0, 184.0 * si.g / si.mol, 0.10]},
    {"API2": [1400 * si.kg / si.m**3, 0, 184.0 * si.g / si.mol, 0.10]},
    {"LIM1": [1400 * si.kg / si.m**3, 0, 200.0 * si.g / si.mol, 0.10]},
    {"LIM2": [1400 * si.kg / si.m**3, 0, 200.0 * si.g / si.mol, 0.10]},
    {"CO3": [2600 * si.kg / si.m**3, 1, 60.0 * si.g / si.mol, 0.00]},
    {"Na": [2200 * si.kg / si.m**3, 1, 23.0 * si.g / si.mol, 0.00]},
    {"Ca": [2600 * si.kg / si.m**3, 1, 40.0 * si.g / si.mol, 0.00]},
    {"OIN": [2600 * si.kg / si.m**3, 0, 1.0 * si.g / si.mol, 0.10]},
    {"OC": [1400 * si.kg / si.m**3, 0, 1.0 * si.g / si.mol, 0.10]},
    {"BC": [1800 * si.kg / si.m**3, 0, 1.0 * si.g / si.mol, 0.00]},
    {"H2O": [1000 * si.kg / si.m**3, 0, 18.0 * si.g / si.mol, 0.00]},
)


class TestAeroData:
    @staticmethod
    def test_ctor():
        # arrange

        # act
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # assert
        assert sut is not None

    @staticmethod
    def test_spec_by_name_found():
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act
        value = sut.spec_by_name("H2O")

        # assert
        assert value == 0

    @staticmethod
    def test_spec_by_name_not_found():
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act and assert
        try:
            _ = sut.spec_by_name("XXX")
            assert False
        except RuntimeError as error:
            assert str(error) == "Element not found."

    @staticmethod
    def test_len():
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act
        value = len(sut)

        # assert
        assert value == len(AERO_DATA_CTOR_ARG_MINIMAL)

    @staticmethod
    def test_frac_dim():
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        value = 3

        # act
        sut.frac_dim = value

        # assert
        assert value == sut.frac_dim

    @staticmethod
    def test_vol_fill_factor():
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        value = 1

        # act
        sut.vol_fill_factor = value

        # assert
        assert value == sut.vol_fill_factor

    @staticmethod
    def test_prime_radius():
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        value = 44

        # act
        sut.prime_radius = value

        # assert
        assert value == sut.prime_radius

    @staticmethod
    def test_rad2vol_sphere():
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        radius = 1e-6

        # act
        value = sut.rad2vol(radius)

        # assert
        np.testing.assert_almost_equal(value, (4 / 3) * np.pi * (radius) ** 3)

    @staticmethod
    @pytest.mark.parametrize(
        "aero_data_params",
        (
            {"frac_dim": 2.4, "vol_fill_factor": 1.2, "prime_radius": 1e-7},
            {"frac_dim": 2.5, "vol_fill_factor": 1.1, "prime_radius": 1e-8},
            {"frac_dim": 2.2, "vol_fill_factor": 1.3, "prime_radius": 1e-6},
        ),
    )
    def test_rad2vol_fractal(aero_data_params: dict):
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        radius = 1e-6
        for key, value in aero_data_params.items():
            setattr(sut, key, value)

        # act
        value = sut.rad2vol(radius)

        # assert
        np.testing.assert_almost_equal(
            value,
            (4 / 3)
            * np.pi
            * (sut.prime_radius) ** 3
            * (radius / sut.prime_radius) ** sut.frac_dim
            / sut.vol_fill_factor,
        )

    @staticmethod
    def test_vol2rad_sphere():
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        vol = 4.19e-18

        # act
        value = sut.vol2rad(vol)

        # assert
        np.testing.assert_almost_equal(value, 1e-6)

    @staticmethod
    @pytest.mark.parametrize(
        "aero_data_params",
        (
            {"frac_dim": 2.4, "vol_fill_factor": 1.2, "prime_radius": 1e-7},
            {"frac_dim": 2.5, "vol_fill_factor": 1.1, "prime_radius": 1e-8},
            {"frac_dim": 2.2, "vol_fill_factor": 1.3, "prime_radius": 1e-6},
        ),
    )
    def test_vol2rad_fractal(aero_data_params: dict):
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        vol = 4.19e-18
        for key, value in aero_data_params.items():
            setattr(sut, key, value)

        # act
        value = sut.vol2rad(vol)

        # assert
        np.testing.assert_almost_equal(
            value,
            sut.prime_radius
            * (
                ((3 * vol / 4 / np.pi) ** (1 / 3) / sut.prime_radius) ** 3
                * sut.vol_fill_factor
            )
            ** (1 / sut.frac_dim),
        )

    @staticmethod
    def test_diam2vol_sphere():
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        diam = 2e-6

        # act
        vol = sut.diam2vol(diam)

        # assert
        np.testing.assert_almost_equal(vol, (np.pi / 6) * diam**3)

    @staticmethod
    @pytest.mark.parametrize(
        "aero_data_params",
        (
            {"frac_dim": 2.4, "vol_fill_factor": 1.2, "prime_radius": 1e-7},
            {"frac_dim": 2.5, "vol_fill_factor": 1.1, "prime_radius": 1e-8},
            {"frac_dim": 2.2, "vol_fill_factor": 1.3, "prime_radius": 1e-6},
        ),
    )
    def test_diam2vol_fractal(aero_data_params: dict):
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        diam = 2e-6
        for key, value in aero_data_params.items():
            setattr(sut, key, value)

        # act
        value = sut.diam2vol(diam)

        # assert
        np.testing.assert_almost_equal(
            value,
            (4 / 3)
            * np.pi
            * (sut.prime_radius) ** 3
            * (1e-6 / sut.prime_radius) ** sut.frac_dim
            / sut.vol_fill_factor,
        )

    @staticmethod
    def test_vol2diam_sphere():
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        vol = 4.19e-18

        # act
        value = sut.vol2diam(vol)

        # arrange
        np.testing.assert_almost_equal(value, 2e-6)

    @staticmethod
    @pytest.mark.parametrize(
        "aero_data_params",
        (
            {"frac_dim": 2.4, "vol_fill_factor": 1.2, "prime_radius": 1e-7},
            {"frac_dim": 2.5, "vol_fill_factor": 1.1, "prime_radius": 1e-8},
            {"frac_dim": 2.2, "vol_fill_factor": 1.3, "prime_radius": 1e-6},
        ),
    )
    def test_vol2diam_fractal(aero_data_params: dict):
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        vol = 4.19e-18
        for key, value in aero_data_params.items():
            setattr(sut, key, value)

        # act
        value = sut.vol2diam(vol)

        # assert
        np.testing.assert_almost_equal(
            value,
            2
            * (
                sut.prime_radius
                * (
                    ((3 * vol / 4 / np.pi) ** (1 / 3) / sut.prime_radius) ** 3
                    * sut.vol_fill_factor
                )
                ** (1 / sut.frac_dim)
            ),
        )

    @staticmethod
    def test_aero_data_densities():
        # arrange
        densities = [1800, 1400, 1800, 1000]
        sut = ppmc.AeroData(
            (
                {
                    "SO4": [
                        densities[0] * si.kg / si.m**3,
                        1,
                        96.0 * si.g / si.mol,
                        0.00,
                    ]
                },
                {
                    "OC": [
                        densities[1] * si.kg / si.m**3,
                        0,
                        1.0 * si.g / si.mol,
                        0.10,
                    ]
                },
                {
                    "BC": [
                        densities[2] * si.kg / si.m**3,
                        0,
                        1.0 * si.g / si.mol,
                        0.00,
                    ]
                },
                {
                    "H2O": [
                        densities[3] * si.kg / si.m**3,
                        0,
                        18.0 * si.g / si.mol,
                        0.00,
                    ]
                },
            )
        )

        # act
        aero_data_densities = sut.densities

        # assert
        assert aero_data_densities == densities

    @staticmethod
    def test_aero_data_density():
        # arrange
        sut = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)

        for item in AERO_DATA_CTOR_ARG_FULL:
            keys = item.keys()
            assert len(keys) == 1
            key = tuple(keys)[0]
            val = tuple(item.values())[0]

            # act
            density = sut.density(key)

            # assert
            assert density == val[0]
