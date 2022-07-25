# pylint: disable=invalid-name,wrong-import-position
import os
from contextlib import contextmanager
from pathlib import Path
from collections import namedtuple

# https://github.com/diegoferigo/cmake-build-extension/blob/master/src/cmake_build_extension/__init__.py
@contextmanager
def __build_extension_env():
    cookies = []
    # https://docs.python.org/3/whatsnew/3.8.html#bpo-36085-whatsnew
    if hasattr(os, "add_dll_directory"):
        basepath = os.path.dirname(os.path.abspath(__file__))
        dllspath = os.path.join(basepath, '..')
        os.environ['PATH'] = dllspath + os.pathsep + os.environ['PATH']
        for path in os.environ.get("PATH", "").split(os.pathsep):
            if path and Path(path).is_absolute() and Path(path).is_dir():
                cookies.append(os.add_dll_directory(path))
    try:
        yield
    finally:
        for cookie in cookies:
            cookie.close()

# TODO #113: 2 x loop over prefixes and units
si = namedtuple("SI", (
    "m", "cm", "um", "nm",
    "kg", "g",
    "s",
    "K",
    "Pa", "hPa",
    "mol"
))(
    m=1., cm=.01, um=1e-6, nm=1e-9,
    kg=1., g=1e-3,
    s=1.,
    K=1.,
    Pa=1., hPa=100.,
    mol=1.
)
""" TODO #113 """

with __build_extension_env():
    from _PyPartMC import *
    from _PyPartMC import __all__, __version__
    import _PyPartMC
