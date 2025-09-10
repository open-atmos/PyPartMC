"""
.. include::../../README.md
"""

#  pylint: disable=invalid-name
import importlib.metadata
import inspect

# pylint: disable=invalid-name,wrong-import-position
import os
from collections import namedtuple
from contextlib import contextmanager
from pathlib import Path

import nanobind


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

from ._PyPartMC import *  # pylint: disable=import-error
from ._PyPartMC import (  # pylint: disable=import-error
    __versions_of_build_time_dependencies__,
)

# Hacky workaround for missing docs in pdoc auto-generated documentation.
# After the switch to nanobind, the docs became empty despite "__doc__" being
# accessible in all of PyPartMC's objects. The code below manually populates
# the "__all__" atrribute of the package. Additionally, functions in the generated
# docs would be listed as nanobind objects with no additional documentation.
# To solve that, dummy functions of the same name are created, and their "__doc__"
# attribute is manually set to the "original" objects' "__doc__"
if os.getenv("PDOC_GENERATE_PYPARTMC_DOCS") == "True":
    all_items = []
    for name, obj in inspect.getmembers(
        _PyPartMC  # pylint: disable=undefined-variable
    ):
        if callable(obj):
            if not inspect.isclass(obj):
                exec(name + " = lambda : 0")  # pylint: disable=exec-used
                temp = "_PyPartMC." + name + ".__doc__"
                setattr(eval(name), "__doc__", eval(temp))  # pylint: disable=eval-used
            all_items.append(name)

    __all__ = tuple([*all_items, "si"])

__version__ = importlib.metadata.version(__package__)

# workaround for MATLAB bindings
# pylint: disable=undefined-variable
setattr(nanobind, "nb_type_0", type(_PyPartMC.AeroData))
