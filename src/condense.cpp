/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "condense.hpp"

void condense_equilib_particle(
    const EnvState &env_state,
    const AeroData &aero_data,
    const AeroParticle &aero_particle
) {
    f_condense_equilib_particle(
        env_state.ptr.f_arg(),
        aero_data.ptr.f_arg(),
        aero_particle.ptr.f_arg()
    );
}

void condense_equilib_particles(
    const EnvState &env_state,
    const AeroData &aero_data,
    const AeroState &aero_state
) {
    f_condense_equilib_particles(
        env_state.ptr.f_arg(),
        aero_data.ptr.f_arg(),
        aero_state.ptr.f_arg()
    );
}
