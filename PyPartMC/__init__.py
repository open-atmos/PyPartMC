# pylint: disable=invalid-name,wrong-import-position
import os
import glob
import sys
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
        print(glob.glob(os.path.join(dllspath, '_PyPartMC*')), file=sys.stderr)
        cookies.append(os.add_dll_directory(dllspath))
        sys.path.append(dllspath)
        
        #os.environ['PATH'] = dllspath + os.pathsep + os.environ['PATH']
        #for path in os.environ.get("PATH", "").split(os.pathsep):
        #    if path and Path(path).is_absolute() and Path(path).is_dir():
        #        print(path, glob.glob(os.path.join(path, '_PyPartMC*')), file=sys.stderr)
        #        cookies.append(os.add_dll_directory(path))
    try:
        print("HERE", file=sys.stderr)
        yield
    finally:
        for cookie in cookies:
            cookie.close()

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

with __build_extension_env():
    from _PyPartMC import *
    from _PyPartMC import __all__, __version__
    import _PyPartMC
