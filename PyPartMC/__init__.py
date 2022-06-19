import sys
import platform

# https://docs.python.org/3/whatsnew/3.8.html#bpo-36085-whatsnew
if (3, 8) <= sys.version_info and platform.system() == 'Windows':
    import os
    import pathlib
    os.add_dll_directory(pathlib.Path(__file__).parent.absolute())

from _PyPartMC import *
from _PyPartMC import __version__
