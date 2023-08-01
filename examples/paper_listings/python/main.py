import numpy as np

import PyPartMC as ppmc
from PyPartMC import si

aero_data = ppmc.AeroData(
    (
        #      [density, ions in solution, molecular weight, kappa]
        {"OC": [1000 * si.kg / si.m**3, 0, 1e-3 * si.kg / si.mol, 0.001]},
        {"BC": [1800 * si.kg / si.m**3, 0, 1e-3 * si.kg / si.mol, 0]},
    )
)

aero_dist = ppmc.AeroDist(
    aero_data,
    [
        {
            "cooking": {
                "mass_frac": [{"OC": [1]}],
                "diam_type": "geometric",
                "mode_type": "log_normal",
                "num_conc": 3200 / si.cm**3,
                "geom_mean_diam": 8.64 * si.nm,
                "log10_geom_std_dev": 0.28,
            }
        },
        {
            "diesel": {
                "mass_frac": [{"OC": [0.3]}, {"BC": [0.7]}],
                "diam_type": "geometric",
                "mode_type": "log_normal",
                "num_conc": 2900 / si.cm**3,
                "geom_mean_diam": 50 * si.nm,
                "log10_geom_std_dev": 0.24,
            }
        },
    ],
)

n_part = 100
seed = 44
offset = 0

aero_state = ppmc.AeroState(n_part, aero_data)

ppmc.rand_init(seed, offset)
aero_state.dist_sample(aero_dist)

print(np.sum(aero_state.masses))
