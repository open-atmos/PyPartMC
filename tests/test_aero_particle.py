####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc

import numpy as np
import pytest

import PyPartMC as ppmc
from PyPartMC import si

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL


class TestAeroParticle:
    @staticmethod
    @pytest.mark.parametrize(
        "volumes",
        (
            [0],
            # pytest.param([],marks=pytest.mark.xfail(strict=True))
        ),
    )
    def test_ctor(volumes):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act
        sut = ppmc.AeroParticle(aero_data, volumes)

        # assert
        assert sut is not None

    @staticmethod
    def test_volumes():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        volumes = [123]

        # act
        sut = ppmc.AeroParticle(aero_data, volumes)

        # assert
        assert sut.volumes == volumes

    @staticmethod
    def test_volume():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        aero_data = None
        gc.collect()

        # act
        vol = sut.volume

        # assert
        assert vol == sum(volumes)

    @staticmethod
    def test_species_volume():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        aero_data = None
        gc.collect()

        # act and assert
        for i, volume in enumerate(volumes):
            vol = sut.species_volume(i)
            assert vol == volume

    @staticmethod
    def test_dry_volume():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        aero_data = None
        volumes = None
        gc.collect()

        # act
        dry_vol = sut.dry_volume

        # assert
        assert dry_vol == 5

    @staticmethod
    def test_particle_radius():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        aero_data.frac_dim = 3.0
        aero_data.vol_fill_factor = 1.0
        aero_data.prime_radius = 1e-8
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        aero_data = None
        volumes = None
        gc.collect()

        # act
        radius = sut.radius

        # assert
        assert radius == ppmc.sphere_vol2rad(6)

    @staticmethod
    def test_dry_radius():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        aero_data.frac_dim = 3.0
        aero_data.vol_fill_factor = 1.0
        aero_data.prime_radius = 1e-8
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        aero_data = None
        volumes = None
        gc.collect()

        # act
        dry_radius = sut.dry_radius

        # assert
        assert dry_radius == ppmc.sphere_vol2rad(5)

    @staticmethod
    def test_diameter():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        aero_data.frac_dim = 3.0
        aero_data.vol_fill_factor = 1.0
        aero_data.prime_radius = 1e-8
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        aero_data = None
        volumes = None
        gc.collect()

        # act
        diameter = sut.diameter

        # assert
        assert diameter == 2 * ppmc.sphere_vol2rad(6)

    @staticmethod
    def test_dry_diameter():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        aero_data.frac_dim = 3.0
        aero_data.vol_fill_factor = 1.0
        aero_data.prime_radius = 1e-8
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        aero_data = None
        volumes = None
        gc.collect()

        # act
        dry_diameter = sut.dry_diameter

        # assert
        assert dry_diameter == 2 * ppmc.sphere_vol2rad(5)

    @staticmethod
    def test_mass():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        aero_data = None
        gc.collect()

        # act
        mass = sut.mass
        check = 0
        for i, spec in enumerate(aero_data_arg):
            key = list(spec)[0]
            check += spec[key][0] * volumes[i]

        # assert
        assert mass == check

    @staticmethod
    def test_species_mass():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        aero_data = None
        gc.collect()

        # act
        sodium_mass = sut.species_mass(2)
        check = aero_data_arg[2]["Na"][0] * volumes[2]

        # assert
        assert sodium_mass == check

    @staticmethod
    def test_species_masses():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        aero_data = None
        gc.collect()

        # act
        masses = sut.species_masses
        check = []
        for i, spec in enumerate(aero_data_arg):
            key = list(spec)[0]
            check.append(spec[key][0] * volumes[i])

        # assert
        assert masses == check

    @staticmethod
    def test_solute_kappa():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        aero_data = None
        volumes = None
        gc.collect()

        # act
        kappa = sut.solute_kappa

        # assert
        np.testing.assert_almost_equal(kappa, 1.479240661)
