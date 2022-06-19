# pylint: disable=invalid-name,wrong-import-position
import os
from contextlib import contextmanager
from pathlib import Path

## https://docs.python.org/3/whatsnew/3.8.html#bpo-36085-whatsnew
#if hasattr(os, 'add_dll_directory'):  # Python 3.8+ on Windows
#    path = os.path.dirname(__file__)
#    print(path)
#    os.add_dll_directory(path)

# https://github.com/diegoferigo/cmake-build-extension/blob/master/src/cmake_build_extension/__init__.py
@contextmanager
def build_extension_env():
    cookies = []
    if hasattr(os, "add_dll_directory"):
        for path in os.environ.get("PATH", "").split(os.pathsep):
            if path and Path(path).is_absolute() and Path(path).is_dir():
                cookies.append(os.add_dll_directory(path))
    try:
        yield
    finally:
        for cookie in cookies:
            cookie.close()

with build_extension_env():
    from _PyPartMC import *
    from _PyPartMC import __version__
