"""
.. include::../README.md
"""

# pylint: disable=invalid-name,wrong-import-position
import os
from collections import namedtuple
from contextlib import contextmanager
from pathlib import Path


# https://github.com/diegoferigo/cmake-build-extension/blob/master/src/cmake_build_extension/__init__.py
@contextmanager
def __build_extension_env():
    cookies = []
    # https://docs.python.org/3/whatsnew/3.8.html#bpo-36085-whatsnew
    if hasattr(os, "add_dll_directory"):
        basepath = os.path.dirname(os.path.abspath(__file__))
        dllspath = os.path.join(basepath, "..")
        os.environ["PATH"] = dllspath + os.pathsep + os.environ["PATH"]
        for path in os.environ.get("PATH", "").split(os.pathsep):
            if path and Path(path).is_absolute() and Path(path).is_dir():
                cookies.append(os.add_dll_directory(path))
    try:
        yield
    finally:
        for cookie in cookies:
            cookie.close()


def __generate_si():
    prefixes = {
        "T": 1e12,
        "G": 1e9,
        "M": 1e6,
        "k": 1e3,
        "h": 1e2,
        "da": 1e1,
        "": 1e0,
        "d": 1e-1,
        "c": 1e-2,
        "m": 1e-3,
        "u": 1e-6,
        "n": 1e-9,
        "p": 1e-12,
    }
    units = {
        "m": 1e0,
        "g": 1e-3,
        "s": 1e0,
        "K": 1e0,
        "Pa": 1e0,
        "mol": 1e0,
        "W": 1e0,
        "J": 1e0,
        "N": 1e0,
    }
    return namedtuple("SI", [prefix + unit for prefix in prefixes for unit in units])(
        **{
            prefix_k + unit_k: prefix_v * unit_v
            for prefix_k, prefix_v in prefixes.items()
            for unit_k, unit_v in units.items()
        }
    )


si = __generate_si()
""" a utility namedtuple aimed at clrifying physics-related code by providing
    SI-prefix-aware unit multipliers, resulting in e.g.: `p = 1000 * si.hPa`
    notation. Note: no dimensional analysis is done! """

with __build_extension_env():
    import _PyPartMC
    from _PyPartMC import *
    from _PyPartMC import __all__ as _PyPartMC_all  # pylint: disable=no-name-in-module
    from _PyPartMC import __version__, __versions_of_build_time_dependencies__

    __all__ = tuple([*_PyPartMC_all, "si"])
