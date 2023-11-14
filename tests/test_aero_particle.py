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
from .test_aero_dist import AERO_DIST_CTOR_ARG_MINIMAL
from .test_aero_state import AERO_STATE_CTOR_ARG_MINIMAL
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL


class TestAeroParticle:  # pylint: disable=too-many-public-methods
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

        for i, volume in enumerate(volumes):
            vol = sut.species_volume(list(aero_data_arg[i])[0])
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

        sodium_mass = sut.species_mass("Na")
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

    @staticmethod
    def test_moles():
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
        moles = sut.moles
        check = 1000 / 18e-3 + 4400 / 35.5e-3 + 6600 / 23e-3

        # assert
        np.testing.assert_almost_equal(moles, check)

    @staticmethod
    def test_mobility_diameter():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        aero_data = None
        volumes = None

        # act
        mobility_diameter = sut.mobility_diameter(env_state)
        env_state = None
        gc.collect()

        # assert
        assert mobility_diameter is not None

    @staticmethod
    def test_density():
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
        density = sut.density

        # assert
        assert density == (1000 * 1 + 2200 * 2 + 2200 * 3) / 6

    @staticmethod
    def test_approx_crit_rel_humid():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        env_state.set_temperature(288)
        aero_data = None
        volumes = None

        # act
        approx_crit_rel_humid = sut.approx_crit_rel_humid(env_state)
        env_state = None
        gc.collect()

        # assert
        assert approx_crit_rel_humid is not None

    @staticmethod
    def test_crit_rel_humid():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        env_state.set_temperature(288)
        aero_data = None
        volumes = None

        # act
        crit_rel_humid = sut.crit_rel_humid(env_state)
        env_state = None
        gc.collect()

        # assert
        assert crit_rel_humid is not None

    @staticmethod
    def test_crit_diameter():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes = [1, 2, 3]
        sut = ppmc.AeroParticle(aero_data, volumes)
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        env_state.set_temperature(288)
        aero_data = None
        volumes = None

        # act
        crit_diameter = sut.crit_diameter(env_state)
        env_state = None
        gc.collect()

        # assert
        assert crit_diameter is not None

    @staticmethod
    def test_coagulate():
        # arrange
        aero_data_arg = (
            {"H2O": [1000 * si.kg / si.m**3, 0, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]},
        )
        aero_data = ppmc.AeroData(aero_data_arg)
        volumes_1 = [1, 2, 3]
        volumes_2 = [3, 2, 1]
        sut = ppmc.AeroParticle(aero_data, volumes_1)
        aero_particle_2 = ppmc.AeroParticle(aero_data, volumes_2)
        aero_data = None
        volumes_1 = None
        volumes_2 = None
        gc.collect()

        # act
        coagulated = sut.coagulate(aero_particle_2)

        # assert
        assert coagulated.volumes == [4, 4, 4]

    @staticmethod
    def test_zero():
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
        sut.zero()

        # assert
        assert sut.volumes == [0, 0, 0]

    @staticmethod
    def test_set_vols():
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
        sut.set_vols([3, 2, 1])

        # assert
        assert sut.volumes == [3, 2, 1]

    @staticmethod
    def test_absorb_cross_sect():
        # arrange
        sut = ppmc.AeroParticle(ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL), [44])

        # act
        value = sut.absorb_cross_sect

        # assert
        assert value == 0
        assert isinstance(value, float)

    @staticmethod
    def test_scatter_cross_sect():
        # arrange
        sut = ppmc.AeroParticle(ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL), [44])

        # act
        value = sut.scatter_cross_sect

        # assert
        assert value == 0
        assert isinstance(value, float)

    @staticmethod
    def test_asymmetry():
        # arrange
        sut = ppmc.AeroParticle(ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL), [44])

        # act
        value = sut.asymmetry

        # assert
        assert value == 0
        assert isinstance(value, float)

    @staticmethod
    def test_refract_shell():
        # arrange
        sut = ppmc.AeroParticle(ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL), [44])

        # act
        value = sut.refract_shell

        # assert
        assert value == 0 + 0j
        assert isinstance(value, complex)

    @staticmethod
    def test_refract_core():
        # arrange
        sut = ppmc.AeroParticle(ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL), [44])

        # act
        value = sut.refract_core

        # assert
        assert value == 0 + 0j
        assert isinstance(value, complex)

    @staticmethod
    def test_n_orig_part():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        aero_state = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
        _ = aero_state.dist_sample(aero_dist, 1.0, 0.0)
        sut = aero_state.particle(0)
        # act
        n_orig_part = sut.n_orig_part

        # assert
        assert len(n_orig_part) == aero_dist.n_mode
        assert isinstance(n_orig_part[0], int)

    @staticmethod
    def test_least_create_time():
        # arrange
        create_time = 44.0
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        aero_state = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
        _ = aero_state.dist_sample(aero_dist, 1.0, create_time)

        # act
        time = []
        for i_part in range(len(aero_state)):
            time.append(aero_state.particle(i_part).least_create_time)

        # assert
        assert np.all(np.isclose(time, create_time))
        assert isinstance(time[0], float)

    @staticmethod
    def test_greatest_create_time():
        # arrange
        create_time = 44.0
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        aero_state = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
        _ = aero_state.dist_sample(aero_dist, 1.0, create_time)

        # act
        time = []
        for i_part in range(len(aero_state)):
            time.append(aero_state.particle(i_part).greatest_create_time)

        # assert
        assert np.all(np.isclose(time, create_time))
        assert isinstance(time[0], float)

    @staticmethod
    def test_id():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        aero_state = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
        _ = aero_state.dist_sample(aero_dist, 1.0, 0.0)

        # act
        ids = []
        for i_part in range(len(aero_state)):
            ids.append(aero_state.particle(i_part).id)

        # assert
        assert isinstance(ids[0], int)
        assert min(ids) > 0
        assert len(np.unique(ids)) == len(aero_state)
