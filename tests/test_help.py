import inspect
import pydoc

import pytest

import PyPartMC as ppmc


@pytest.mark.parametrize(
    "obj",
    (
        ppmc.AeroBinned,
        ppmc.AeroData,
        ppmc.AeroParticle,
        ppmc.AeroState,
        ppmc.CampCore,
        ppmc.EnvState,
        ppmc.GasData,
        ppmc.GasState,
        ppmc.Photolysis,
        ppmc.RunExactOpt,
        ppmc.RunPartOpt,
        ppmc.RunSectOpt,
        ppmc.Scenario,
        ppmc.condense_equilib_particle,
        ppmc.condense_equilib_particles,
        ppmc.diam2rad,
        ppmc.histogram_1d,
        ppmc.histogram_2d,
        ppmc.input_exact,
        ppmc.input_sectional,
        ppmc.input_state,
        ppmc.loss_rate,
        ppmc.loss_rate_dry_dep,
        ppmc.output_state,
        ppmc.pow2_above,
        ppmc.rad2diam,
        ppmc.rand_init,
        ppmc.rand_normal,
        ppmc.run_exact,
        ppmc.run_part,
        ppmc.run_part_timeblock,
        ppmc.run_part_timestep,
        ppmc.run_sect,
        ppmc.sphere_rad2vol,
        ppmc.sphere_vol2rad,
    ),
)
def test_help_output(obj):
    help_output = pydoc.render_doc(obj, renderer=pydoc.plaintext)

    assert len(obj.__doc__) > 0

    # help() output of functions is broken on CI
    if inspect.isclass(obj):
        # remove whitespace and bars
        assert "".join(obj.__doc__.split()) in "".join(
            help_output.replace("|", "").split()
        )
