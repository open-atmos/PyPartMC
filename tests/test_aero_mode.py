####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
####################################################################################################

import numpy as np
import pytest

import PyPartMC as ppmc
from PyPartMC import si

from .test_aero_data import AERO_DATA_CTOR_ARG_MINIMAL

AERO_MODE_CTOR_LOG_NORMAL = {
    "test_mode": {
        "mass_frac": [{"H2O": [1]}],
        "diam_type": "geometric",
        "mode_type": "log_normal",
        "num_conc": 100 / si.m**3,
        "geom_mean_diam": 2 * si.um,
        "log10_geom_std_dev": np.log10(1.6),
    }
}


class TestAeroMode:
    @staticmethod
    def test_ctor():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)

        # act
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)

        # assert
        assert sut is not None

    @staticmethod
    def test_set_vol_frac():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)
        vals = [1.0]

        # act
        sut.vol_frac = vals

        # assert
        assert sut.vol_frac == vals

    @staticmethod
    def test_set_num_conc():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)
        val = 1234

        # act
        sut.num_conc = val

        # assert
        assert sut.num_conc == val

    @staticmethod
    def test_set_gsd():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)
        val = 1.234

        # act
        sut.gsd = val

        # assert
        assert sut.gsd == val

    @staticmethod
    def test_set_char_radius():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)
        val = 1e-9

        # act
        sut.char_radius = val

        # assert
        assert sut.char_radius == val

    @staticmethod
    def test_set_vol_frac_invalid():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)

        # act
        try:
            sut.vol_frac = [1.0, 0.0]
        except RuntimeError:
            return

        # assert
        assert False

    @staticmethod
    def test_set_sample_invalid():
        # arrange
        aero_data = ppmc.AeroData(AERO_DATA_CTOR_ARG_MINIMAL)
        sut = ppmc.AeroMode(aero_data, AERO_MODE_CTOR_LOG_NORMAL)

        # act
        try:
            sut.set_sample([1, 10], [10, 10])
        except RuntimeError:
            return

        # assert
        assert False

    @staticmethod
    @pytest.mark.parametrize(
        "modes",
        (
            {
                "test_mono": {
                    "mass_frac": [
                        {"A": [0.2]},
                        {"B": [0.3]},
                        {"C": [0.1]},
                        {"D": [0.4]},
                    ],
                    "mode_type": "mono",
                    "num_conc": 100 / si.m**3,
                    "diam": 2 * si.um,
                }
            },
            {
                "test_logn": {
                    "mass_frac": [
                        {"A": [0.3]},
                        {"B": [0.1]},
                        {"C": [0.1]},
                        {"D": [0.5]},
                    ],
                    "mode_type": "log_normal",
                    "num_conc": 100 / si.m**3,
                    "geom_mean_diam": 2 * si.um,
                    "log10_geom_std_dev": 1.6,
                }
            },
            {
                "test_exp": {
                    "mass_frac": [
                        {"A": [0.6]},
                        {"B": [0.1]},
                        {"C": [0.1]},
                        {"D": [0.2]},
                    ],
                    "mode_type": "exp",
                    "num_conc": 100 / si.m**3,
                    "diam_at_mean_vol": 2 * si.um,
                    "temp": 300 * si.K,
                }
            },
        ),
    )
    @pytest.mark.parametrize("diam_type", ("geometric", "mobility"))
    def test_from_json_mono(modes, diam_type):
        # arrange
        densities = [
            {"A": [1234 * si.kg / si.m**3, 1, 18e-3 * si.kg / si.mol, 0]},
            {"B": [4321 * si.kg / si.m**3, 1, 18e-3 * si.kg / si.mol, 0]},
            {"C": [1111 * si.kg / si.m**3, 1, 18e-3 * si.kg / si.mol, 0]},
            {"D": [2222 * si.kg / si.m**3, 1, 18e-3 * si.kg / si.mol, 0]},
        ]
        aero_data = ppmc.AeroData(densities)
        for mode in modes.values():
            mode["diam_type"] = diam_type
            if diam_type == "mobility":
                mode["pressure"] = 1000 * si.hPa
                if "temp" not in mode:
                    mode["temp"] = 299 * si.K

        # act
        sut = ppmc.AeroMode(aero_data, modes)

        # assert
        mode = modes[tuple(modes.keys())[0]]
        assert sut.num_conc == mode["num_conc"]
        assert sut.type == mode["mode_type"]
