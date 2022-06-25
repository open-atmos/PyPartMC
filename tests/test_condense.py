####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import numpy as np
import PyPartMC as ppmc
from PyPartMC import si
from .test_env_state import ENV_STATE_CTOR_ARG_MINIMAL
from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL
from .test_aero_state import AERO_STATE_CTOR_ARG_MINIMAL

# pylint: disable=unused-variable


class TestCondense:
    @staticmethod
    def test_equilib_particles():
        # arrange
        env_state = ppmc.EnvState(ENV_STATE_CTOR_ARG_MINIMAL)
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        aero_state = ppmc.AeroState(AERO_STATE_CTOR_ARG_MINIMAL, aero_data)

        # act
        ppmc.condense_equilib_particles(env_state, aero_data, aero_state)

        # assert
        pass  # TODO

    @staticmethod
    def test_equilib_particle():
        # arrange
        env_state_ctor_arg = ENV_STATE_CTOR_ARG_MINIMAL
        env_state_ctor_arg["rel_humid"] = .99
        env_state = ppmc.EnvState(env_state_ctor_arg)
        aero_data = ppmc.AeroData((
            {"H2O": [1000 * si.kg / si.m**3, 1, 18e-3 * si.kg / si.mol, 0]},
            {"Cl": [2200 * si.kg / si.m**3, 1, 35.5e-3 * si.kg / si.mol, 0]},
            {"Na": [2200 * si.kg / si.m**3, 1, 23e-3 * si.kg / si.mol, 0]}
        ))
        aero_particle = ppmc.AeroParticle(aero_data, [0, 1000 * si.nm**3, 1000 * si.nm**3])

        # act
        ppmc.condense_equilib_particle(env_state, aero_data, aero_particle)

        # assert
        np.testing.assert_almost_equal(4.95452639e-23, tuple(aero_particle.volumes)[0])
