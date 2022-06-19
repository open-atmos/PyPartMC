# pylint: disable=invalid-name,wrong-import-position
import os

# https://docs.python.org/3/whatsnew/3.8.html#bpo-36085-whatsnew
if hasattr(os, 'add_dll_directory'):  # Python 3.8+ on Windows
    path = os.path.dirname(__file__)
    print(path)
    os.add_dll_directory(path)

from _PyPartMC import *
from _PyPartMC import __version__
