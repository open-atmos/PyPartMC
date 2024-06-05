####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import gc
import platform

import numpy as np
import pytest

import PyPartMC as ppmc

from .test_aero_data import AERO_DATA_CTOR_ARG_FULL, AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_dist import (
    AERO_DIST_CTOR_ARG_AVERAGE,
    AERO_DIST_CTOR_ARG_FULL,
    AERO_DIST_CTOR_ARG_MINIMAL,
)
from .test_aero_mode import AERO_MODE_CTOR_SAMPLED
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL

AERO_STATE_CTOR_ARG_MINIMAL = 44, "nummass_source"

# pylint: disable=R0904


@pytest.fixture(name="sut_minimal")
def sut_minimal_fixture():
    aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
    aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
    sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
    _ = sut.dist_sample(aero_dist, 1.0, 0.0, True, True)
    aero_data = None
    gc.collect()
    return sut


@pytest.fixture(name="sut_full")
def sut_full_fixture():
    aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)
    aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_FULL)
    sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
    _ = sut.dist_sample(aero_dist, 1.0, 0.0, True, True)
    aero_data = None
    gc.collect()
    return sut


@pytest.fixture(name="sut_average")
def sut_average_fixture():
    aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_FULL)
    aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_AVERAGE)
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
    @pytest.mark.skipif(platform.machine() == "arm64", reason="TODO #348")
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
    def test_total_num_conc(sut_minimal):
        # act
        total_num_conc = sut_minimal.total_num_conc

        # assert
        assert isinstance(total_num_conc, float)
        assert total_num_conc > 0

    @staticmethod
    def test_total_mass_conc(sut_minimal):
        # act
        total_mass_conc = sut_minimal.total_mass_conc

        # assert
        assert isinstance(total_mass_conc, float)
        assert total_mass_conc > 0

    @staticmethod
    def test_num_concs(sut_minimal):
        # act
        num_concs = sut_minimal.num_concs

        # assert
        assert isinstance(num_concs, list)
        assert len(num_concs) == len(sut_minimal)

    @staticmethod
    def test_masses(sut_minimal):
        # act
        masses = sut_minimal.masses()

        # assert
        assert isinstance(masses, list)
        assert len(masses) == len(sut_minimal)

    @staticmethod
    def test_masses_include(sut_full):
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
    def test_masses_exclude(sut_full):
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
    def test_masses_include_exclude(sut_full):
        # act
        masses = sut_full.masses(include=["SO4"], exclude=["SO4"])

        # assert
        assert isinstance(masses, list)
        assert len(masses) == len(sut_full)
        assert np.sum(masses) == 0.0

    @staticmethod
    def test_volumes(sut_minimal):
        # act
        volumes = sut_minimal.volumes()

        # assert
        assert isinstance(volumes, list)
        assert len(volumes) == len(sut_minimal)

    @staticmethod
    def test_volumes_include(sut_full):
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
    def test_volumes_include_exclude(sut_full):
        # act
        volumes = sut_full.volumes(include=["SO4"], exclude=["SO4"])

        # assert
        assert isinstance(volumes, list)
        assert len(volumes) == len(sut_full)
        assert np.sum(volumes) == 0.0

    @staticmethod
    def test_volumes_exclude(sut_full):
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
    def test_dry_diameters(sut_minimal):
        # act
        dry_diameters = sut_minimal.dry_diameters

        # assert
        assert isinstance(dry_diameters, list)
        assert len(dry_diameters) == len(sut_minimal)

    @staticmethod
    def test_mobility_diameters(sut_minimal):
        # act
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        env_state.set_temperature(300)
        env_state.pressure = 1e5
        diameters = sut_minimal.mobility_diameters(env_state)

        # assert
        assert isinstance(diameters, list)
        assert len(diameters) == len(sut_minimal)
        assert (np.asarray(diameters) > 0).all()

    @staticmethod
    def test_diameters(sut_minimal):
        # act
        diameters = sut_minimal.diameters()

        # assert
        assert isinstance(diameters, list)
        assert len(diameters) == len(sut_minimal)

    @staticmethod
    def test_ids(sut_minimal):
        # act
        ids = sut_minimal.ids

        # assert
        assert isinstance(ids, list)
        assert len(ids) == len(sut_minimal)
        assert (np.asarray(ids) > 0).all()

    @staticmethod
    def test_crit_rel_humids(sut_full):
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
    def test_make_dry(sut_minimal):
        # act
        sut_minimal.make_dry()

        masses = sut_minimal.masses(include=["H2O"])
        assert (np.asarray(masses) == 0).all()

    @staticmethod
    def test_mixing_state(sut_minimal):
        # act
        mixing_state = sut_minimal.mixing_state()

        # assert
        assert isinstance(mixing_state, tuple)
        assert len(mixing_state) == 3

    @staticmethod
    @pytest.mark.parametrize("n_bin", (1, 123))
    def test_bin_average_comp(sut_average, n_bin):
        # arrange
        bin_grid = ppmc.BinGrid(n_bin, "log", 1e-9, 1e-4)

        # act
        sut_average.bin_average_comp(bin_grid)
        so4_masses = np.array(sut_average.masses(include=["SO4"]))
        bc_masses = np.array(sut_average.masses(include=["BC"]))

        # assert
        if n_bin == 1:
            assert np.all(
                np.isclose(so4_masses / bc_masses, so4_masses[0] / bc_masses[0])
            )
        else:
            assert np.logical_xor(so4_masses > 0, bc_masses > 0).all()

    @staticmethod
    def test_get_particle(sut_minimal):
        # act
        particle = sut_minimal.particle(1)

        # assert
        assert isinstance(particle, ppmc.AeroParticle)

    @staticmethod
    def test_get_random_particle(sut_minimal):
        # act
        particle = sut_minimal.rand_particle()

        # assert
        assert isinstance(particle, ppmc.AeroParticle)

    @staticmethod
    def test_check_correct_particle(sut_minimal):
        # act
        i_part = 20
        particle = sut_minimal.particle(i_part)
        diameters = sut_minimal.diameters()

        # assert
        assert particle.diameter == diameters[i_part]

    @staticmethod
    def test_different_particles(sut_minimal):
        # act
        i_part = 20
        particle_1 = sut_minimal.particle(i_part)
        particle_2 = sut_minimal.particle(i_part + 1)

        # assert
        assert particle_1.diameter != particle_2.diameter

    @staticmethod
    @pytest.mark.parametrize("idx", (-1, 500))
    @pytest.mark.skipif(platform.machine() == "arm64", reason="TODO #348")
    def test_get_particle_out_of_range(sut_minimal, idx):
        # act
        try:
            _ = sut_minimal.particle(idx)
        except IndexError:
            return

        # assert
        assert False

    @staticmethod
    def test_remove_particle(sut_minimal):
        diameters = sut_minimal.diameters()
        sut_minimal.remove_particle(len(sut_minimal) - 1)

        assert diameters[0:-1] == sut_minimal.diameters()

    @staticmethod
    def test_zero(sut_minimal):
        # act
        sut_minimal.zero()

        # assert
        assert len(sut_minimal) == 0

    @staticmethod
    def test_add_particle(sut_minimal):
        particle = sut_minimal.particle(1)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)

        sut.add_particle(particle)

        assert len(sut) == 1
        assert sut.particle(0).diameter == sut_minimal.particle(1).diameter

    @staticmethod
    def test_add_particles(sut_minimal):
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        delta = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
        _ = delta.dist_sample(aero_dist, 1.0, 0.0, True, True)

        num_conc = sut_minimal.total_num_conc
        num_conc_delta = delta.total_num_conc
        sut_minimal.add_particles(delta)

        assert np.isclose(sut_minimal.total_num_conc, (num_conc + num_conc_delta))

    @staticmethod
    def test_add(sut_minimal):
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, AERO_DIST_CTOR_ARG_MINIMAL)
        delta = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
        _ = delta.dist_sample(aero_dist, 1.0, 0.0, True, True)

        num_conc = sut_minimal.total_num_conc
        num_conc_delta = delta.total_num_conc
        sut_minimal.add(delta)

        assert np.isclose(sut_minimal.total_num_conc, 0.5 * (num_conc + num_conc_delta))

    @staticmethod
    def test_sample_particles(sut_minimal):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)
        sut.copy_weight(sut_minimal)

        # act
        num_conc = sut_minimal.total_num_conc
        samp_prob = 0.25
        sut_minimal.sample_particles(sut, samp_prob)

        # assert
        assert len(sut) > 0
        assert sut.total_num_conc > 0.5 * samp_prob * num_conc
        assert sut.total_num_conc < sut_minimal.total_num_conc
        assert np.isclose(sut.total_num_conc + sut_minimal.total_num_conc, num_conc)

    @staticmethod
    def test_sample(sut_minimal):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)

        # act
        num_conc = sut_minimal.total_num_conc
        samp_prob = 0.1
        sut_minimal.sample(sut, samp_prob)

        # assert
        assert len(sut) > 0
        assert sut.total_num_conc > 0.5 * samp_prob * num_conc
        assert np.isclose(
            (
                samp_prob * sut.total_num_conc
                + (1 - samp_prob) * sut_minimal.total_num_conc
            ),
            num_conc,
        )

    @staticmethod
    def test_copy_weight(sut_minimal):
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)

        sut.copy_weight(sut_minimal)
        sut.add_particle(sut_minimal.particle(0))

        # pylint: disable=unsubscriptable-object
        assert sut.num_concs[0] == sut_minimal.num_concs[0]

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

    @staticmethod
    def test_dist_sample_sampled():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, [AERO_MODE_CTOR_SAMPLED])
        sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)

        # act
        _ = sut.dist_sample(aero_dist, 1.0, 0.0, True, True)

        # assert
        assert (
            np.array(sut.diameters())
            >= AERO_MODE_CTOR_SAMPLED["test_mode"]["size_dist"][0]["diam"][0]
        ).all()
        assert (
            np.array(sut.diameters())
            <= AERO_MODE_CTOR_SAMPLED["test_mode"]["size_dist"][0]["diam"][-1]
        ).all()

    @staticmethod
    def test_dist_sample_mono():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        diam = 2e-6
        aero_dist = ppmc.AeroDist(
            aero_data,
            [
                {
                    "test_mode": {
                        "mass_frac": [{"H2O": [1]}],
                        "diam_type": "geometric",
                        "mode_type": "mono",
                        "num_conc": 1e12,
                        "diam": diam,
                    }
                }
            ],
        )
        sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)

        # act
        _ = sut.dist_sample(aero_dist, 1.0, 0.0, True, True)

        # assert
        assert np.isclose(np.array(sut.diameters()), diam).all()

    @staticmethod
    @pytest.mark.parametrize(
        "args",
        (
            ((True, True), (True, False)),
            ((True, True), (False, True)),
            ((True, True), (False, False)),
            ((False, False), (True, False)),
            ((False, False), (False, True)),
            ((False, False), (True, True)),
            ((True, False), (False, False)),
            ((True, False), (False, True)),
            ((False, True), (False, False)),
            ((False, True), (True, False)),
        ),
    )
    @pytest.mark.skipif(platform.machine() == "arm64", reason="TODO #348")
    def test_dist_sample_different_halving(args):
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_dist = ppmc.AeroDist(aero_data, [AERO_MODE_CTOR_SAMPLED])
        sut = ppmc.AeroState(aero_data, *AERO_STATE_CTOR_ARG_MINIMAL)

        # act
        with pytest.raises(RuntimeError) as excinfo:
            _ = sut.dist_sample(aero_dist, 1.0, 0.0, *args[0])
            _ = sut.dist_sample(aero_dist, 1.0, 0.0, *args[1])

        # assert
        assert (
            str(excinfo.value)
            == "dist_sample() called with different halving/doubling settings then in last call"
        )
