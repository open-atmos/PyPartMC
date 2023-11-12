####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc

import numpy as np
import pytest

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_FULL, AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_dist import AERO_DIST_CTOR_ARG_FULL, AERO_DIST_CTOR_ARG_MINIMAL
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL

AERO_STATE_CTOR_ARG_MINIMAL = 44, "nummass_source"

# pylint: disable=R0904


@pytest.fixture
def sut_minimal():
    aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
    aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
    sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
    _ = sut.dist_sample(aero_dist, 1.0, 0.0, True, True)
    aero_data = None
    gc.collect()
    return sut


@pytest.fixture
def sut_full():
    aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)
    aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_FULL)
    sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
    _ = sut.dist_sample(aero_dist, 1.0, 0.0, True, True)
    aero_data = None
    gc.collect()
    return sut


class TestAeroState:
    @staticmethod
    def test_ctor():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act
        sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)

        # assert
        assert sut is not None

    @staticmethod
    def test_ctor_fails_on_unknown_weighting():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        name = "kopytko"

        # act
        with pytest.raises(RuntimeError) as excinfo:
            _ = ppmc.AeroState(aero_data, 1, name)

        # assert
        assert (
            str(excinfo.value)
            == f"unknown weighting scheme '{name}', valid options are: "
            + "flat, flat_source, nummass, nummass_source"
        )

    @staticmethod
    @pytest.mark.parametrize("n_part", (44, 666))
    def test_len(n_part):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroState(aero_data, n_part, "nummass_source")
        _ = sut.dist_sample(aero_dist, 1.0, 0.0, True, True)

        # act
        size = len(sut)

        # assert
        assert int(size) > n_part * 0.5
        assert int(size) < n_part * 2

    @staticmethod
    def test_total_num_conc(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        total_num_conc = sut_minimal.total_num_conc

        # assert
        assert isinstance(total_num_conc, float)
        assert total_num_conc > 0

    @staticmethod
    def test_total_mass_conc(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        total_mass_conc = sut_minimal.total_mass_conc

        # assert
        assert isinstance(total_mass_conc, float)
        assert total_mass_conc > 0

    @staticmethod
    def test_num_concs(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        num_concs = sut_minimal.num_concs

        # assert
        assert isinstance(num_concs, list)
        assert len(num_concs) == len(sut_minimal)

    @staticmethod
    def test_masses(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        masses = sut_minimal.masses()

        # assert
        assert isinstance(masses, list)
        assert len(masses) == len(sut_minimal)

    @staticmethod
    def test_masses_include(sut_full):  # pylint: disable=redefined-outer-name
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)

        # act
        so4_ind = aero_data.spec_by_name("SO4")
        masses = sut_full.masses(include=["SO4"])
        masses_so4 = np.zeros(len(sut_full))
        for i_part in range(len(sut_full)):
            masses_so4[i_part] = sut_full.particle(i_part).species_masses[so4_ind]

        # assert
        assert isinstance(masses, list)
        assert len(masses) == len(sut_full)
        np.testing.assert_allclose(masses_so4, masses)

    @staticmethod
    def test_masses_exclude(sut_full):  # pylint: disable=redefined-outer-name
        # act
        masses = sut_full.masses(exclude=["SO4"])
        masses_so4 = np.zeros(len(sut_full))
        for i_part in range(len(sut_full)):
            masses_so4[i_part] = sut_full.particle(i_part).species_mass(1)

        # assert
        assert isinstance(masses, list)
        assert len(masses) == len(sut_full)
        np.testing.assert_allclose(masses_so4, masses)

    @staticmethod
    def test_masses_include_exclude(sut_full):  # pylint: disable=redefined-outer-name
        # act
        masses = sut_full.masses(include=["SO4"], exclude=["SO4"])

        # assert
        assert isinstance(masses, list)
        assert len(masses) == len(sut_full)
        assert np.sum(masses) == 0.0

    @staticmethod
    def test_volumes(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        volumes = sut_minimal.volumes()

        # assert
        assert isinstance(volumes, list)
        assert len(volumes) == len(sut_minimal)

    @staticmethod
    def test_volumes_include(sut_full):  # pylint: disable=redefined-outer-name
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)

        # act
        so4_ind = aero_data.spec_by_name("SO4")
        volumes = sut_full.volumes(include=["SO4"])
        vol_so4 = np.zeros(len(sut_full))
        for i_part in range(len(sut_full)):
            vol_so4[i_part] = sut_full.particle(i_part).volumes[so4_ind]

        # assert
        assert isinstance(volumes, list)
        assert len(volumes) == len(sut_full)
        np.testing.assert_allclose(vol_so4, volumes)

    @staticmethod
    def test_volumes_include_exclude(sut_full):  # pylint: disable=redefined-outer-name
        # act
        volumes = sut_full.volumes(include=["SO4"], exclude=["SO4"])

        # assert
        assert isinstance(volumes, list)
        assert len(volumes) == len(sut_full)
        assert np.sum(volumes) == 0.0

    @staticmethod
    def test_volumes_exclude(sut_full):  # pylint: disable=redefined-outer-name
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)

        # act
        so4_ind = aero_data.spec_by_name("SO4")
        volumes = sut_full.volumes(exclude=["SO4"])
        vol_so4 = np.zeros(len(sut_full))
        for i_part in range(len(sut_full)):
            vol_so4[i_part] = (
                np.sum(sut_full.particle(i_part).volumes)
                - sut_full.particle(i_part).volumes[so4_ind]
            )

        # assert
        assert isinstance(volumes, list)
        assert len(volumes) == len(sut_full)
        np.testing.assert_allclose(vol_so4, volumes)

    @staticmethod
    def test_dry_diameters(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        dry_diameters = sut_minimal.dry_diameters

        # assert
        assert isinstance(dry_diameters, list)
        assert len(dry_diameters) == len(sut_minimal)

    @staticmethod
    def test_diameters(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        diameters = sut_minimal.diameters()

        # assert
        assert isinstance(diameters, list)
        assert len(diameters) == len(sut_minimal)

    @staticmethod
    def test_crit_rel_humids(sut_full):  # pylint: disable=redefined-outer-name
        # arrange
        args = {"rel_humidity": 0.8, **ENV_STATE_CTOR_ARG_MINIMAL}
        env_state = ppmc.EnvState(args)
        env_state.set_temperature(300)

        # act
        crit_rel_humids = sut_full.crit_rel_humids(env_state)

        # assert
        assert isinstance(crit_rel_humids, list)
        assert len(crit_rel_humids) == len(sut_full)
        assert (np.asarray(crit_rel_humids) > 1).all()
        assert (np.asarray(crit_rel_humids) < 1.2).all()

    @staticmethod
    def test_mixing_state(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        mixing_state = sut_minimal.mixing_state()

        # assert
        assert isinstance(mixing_state, tuple)
        assert len(mixing_state) == 3

    @staticmethod
    def test_bin_average_comp(sut_minimal):  # pylint: disable=redefined-outer-name
        # arrange
        bin_grid = ppmc.BinGrid(123, "log", 1e-9, 1e-4)

        # act
        sut_minimal.bin_average_comp(bin_grid)

        # assert
        # TODO #179

    @staticmethod
    def test_get_particle(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        particle = sut_minimal.particle(1)

        # assert
        assert isinstance(particle, ppmc.AeroParticle)

    @staticmethod
    def test_get_random_particle(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        particle = sut_minimal.rand_particle()

        # assert
        assert isinstance(particle, ppmc.AeroParticle)

    @staticmethod
    def test_check_correct_particle(
        sut_minimal,
    ):  # pylint: disable=redefined-outer-name
        # act
        i_part = 20
        particle = sut_minimal.particle(i_part)
        diameters = sut_minimal.diameters()

        # assert
        assert particle.diameter == diameters[i_part]

    @staticmethod
    def test_different_particles(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        i_part = 20
        particle_1 = sut_minimal.particle(i_part)
        particle_2 = sut_minimal.particle(i_part + 1)

        # assert
        assert particle_1.diameter != particle_2.diameter

    @staticmethod
    @pytest.mark.parametrize("idx", (-1, 500))
    def test_get_particle_out_of_range(
        sut_minimal, idx
    ):  # pylint: disable=redefined-outer-name
        # act
        try:
            _ = sut_minimal.particle(idx)
        except IndexError:
            return

        # assert
        assert False

    @staticmethod
    @pytest.mark.parametrize(
        "args",
        (
            (1.0, 0.0, True, True),
            pytest.param((), id="default args"),
        ),
    )
    @pytest.mark.parametrize(
        "weighting",
        (
            "flat_source",
            "flat",
            "nummass_source",
            "nummass",
        ),
    )
    def test_dist_sample(args, weighting):
        # arrange
        n_part = 44
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroState(aero_data, n_part, weighting)

        # act
        n_added = sut.dist_sample(aero_dist, *args)

        # assert
        assert n_added > n_part * 0.5
        assert n_added < n_part * 2
