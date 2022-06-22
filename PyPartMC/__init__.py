# pylint: disable=invalid-name,wrong-import-position
import os
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
    basepath = os.path.dirname(os.path.abspath(__file__))
    dllspath = os.path.join(basepath, '..')
    os.add_dll_directory(dllspath)

from _PyPartMC import *
from _PyPartMC import __all__
import _PyPartMC
