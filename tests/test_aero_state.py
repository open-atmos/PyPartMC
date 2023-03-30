####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc
from copy import deepcopy

import numpy as np
import pytest

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_FULL, AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_dist import AERO_DIST_CTOR_ARG_FULL, AERO_DIST_CTOR_ARG_MINIMAL
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL

AERO_STATE_CTOR_ARG_MINIMAL = 44


@pytest.fixture
def sut_minimal():
    aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
    aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
    sut = ppmc.AeroState(AERO_STATE_CTOR_ARG_MINIMAL, aero_data)
    _ = sut.dist_sample(aero_dist, 1.0, 0.0, True, True)
    aero_data = None
    gc.collect()
    return sut


@pytest.fixture
def sut_full():
    aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)
    aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_FULL)
    sut = ppmc.AeroState(AERO_STATE_CTOR_ARG_MINIMAL, aero_data)
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
        sut = ppmc.AeroState(AERO_STATE_CTOR_ARG_MINIMAL, aero_data)

        # assert
        assert sut is not None

    @staticmethod
    @pytest.mark.parametrize("n_part", (44, 666))
    def test_len(n_part):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroState(n_part, aero_data)
        _ = sut.dist_sample(aero_dist, 1.0, 0.0, True, True)

        # act
        size = len(sut)

        # assert
        assert int(size) > n_part * 0.5
        assert int(size) < n_part * 2

    @staticmethod
    def test_copy(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        aero_state_copy = deepcopy(sut_minimal)

        # assert
        assert aero_state_copy is not sut_minimal
        expected = np.sum(sut_minimal.diameters)
        actual = np.sum(aero_state_copy.diameters)
        assert expected == actual

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
        masses = sut_minimal.masses

        # assert
        assert isinstance(masses, list)
        assert len(masses) == len(sut_minimal)

    @staticmethod
    def test_volumes(sut_minimal):  # pylint: disable=redefined-outer-name
        # act
        volumes = sut_minimal.volumes

        # assert
        assert isinstance(volumes, list)
        assert len(volumes) == len(sut_minimal)

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
        diameters = sut_minimal.diameters

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
        mixing_state = sut_minimal.mixing_state

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
        diameters = sut_minimal.diameters

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
    def test_dist_sample():
        n_part = 44
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroState(n_part, aero_data)
        n_added = sut.dist_sample(aero_dist, 1.0, 0.0, True, True)

        assert n_added > n_part * 0.5
        assert n_added < n_part * 2
