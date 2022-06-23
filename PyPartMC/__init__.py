# pylint: disable=invalid-name,wrong-import-position
import os
from contextlib import contextmanager
from pathlib import Path
from collections import namedtuple

# TODO: 2 x loop over prefixes and units
si = namedtuple("SI", (
    "m", "cm", "um",
    "kg", "g",
    "s",
    "K",
    "Pa", "hPa",
    "mol"
))(
    m=1., cm=.01, um=1e-6,
    kg=1., g=1e-3,
    s=1.,
    K=1.,
    Pa=1., hPa=100.,
    mol=1.
)
""" TODO """

# https://docs.python.org/3/whatsnew/3.8.html#bpo-36085-whatsnew
if hasattr(os, "add_dll_directory"):
    __dllspath = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
    os.environ['PATH'] = __dllspath + os.pathsep + os.environ['PATH']
    __cookies = []
    for path in os.environ.get("PATH", "").split(os.pathsep):
        if path and Path(path).is_absolute() and Path(path).is_dir():
            __cookies.append(os.add_dll_directory(path))

from _PyPartMC import *
from _PyPartMC import __all__, __version__
import _PyPartMC

if hasattr(os, "add_dll_directory"):
    for cookie in __cookies:
        cookie.close()

