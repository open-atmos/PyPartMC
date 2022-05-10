# PyPartMC (pre-alpha!)

[![Python 3](https://img.shields.io/static/v1?label=Python&logo=Python&color=3776AB&message=3)](https://www.python.org/)
[![Linux OK](https://img.shields.io/static/v1?label=Linux&logo=Linux&color=yellow&message=%E2%9C%93)](https://en.wikipedia.org/wiki/Linux)
[![macOS OK](https://img.shields.io/static/v1?label=macOS&logo=Apple&color=silver&message=%E2%9C%93)](https://en.wikipedia.org/wiki/macOS)
[![Windows OK](https://img.shields.io/static/v1?label=Windows&logo=Windows&color=white&message=%E2%9C%93)](https://en.wikipedia.org/wiki/Windows)
[![Jupyter](https://img.shields.io/static/v1?label=Jupyter&logo=Jupyter&color=f37626&message=%E2%9C%93)](https://jupyter.org/)

[![US Funding](https://img.shields.io/static/v1?label=US%20DOE%20Funding%20by&color=267c32&message=ASR&logoWidth=25&logo=image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAB4AAAAQCAMAAAA25D/gAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAASFBMVEVOTXyyIjRDQnNZWINZWITtzdFUU4BVVIFVVYHWiZM9PG/KZnNXVoJaWYT67/FKSXhgX4hgX4lcW4VbWoX03uHQeIN2VXj///9pZChlAAAAAWJLR0QXC9aYjwAAAAd0SU1FB+EICRMGJV+KCCQAAABdSURBVBjThdBJDoAgEETRkkkZBBX0/kd11QTTpH1/STqpAAwWBkobSlkGbt0o5xmEfqxDZJB2Q6XMoBwnVSbTylWp0hi42rmbwTOYPDfR5Kc+07IIUQQvghX9THsBHcES8/SiF0kAAAAldEVYdGRhdGU6Y3JlYXRlADIwMTctMDgtMDlUMTk6MDY6MzcrMDA6MDCX1tBgAAAAJXRFWHRkYXRlOm1vZGlmeQAyMDE3LTA4LTA5VDE5OjA2OjM3KzAwOjAw5oto3AAAAABJRU5ErkJggg==)](https://asr.science.energy.gov/)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![Copyright](https://img.shields.io/static/v1?label=Copyright&color=249fe2&message=UIUC&)](https://atmos.illinois.edu/)

[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://GitHub.com/slayoo/PyPartMC/graphs/commit-activity)
[![Github Actions Build Status](https://github.com/slayoo/PyPartMC/workflows/tests/badge.svg?branch=main)](https://github.com/slayoo/PyPartMC/actions)
[![API docs](https://img.shields.io/badge/API_docs-pdoc3-blue.svg)](https://slayoo.github.io/PyPartMC/)


PyPartMC is a Python interface to [PartMC](https://lagrange.mechse.illinois.edu/partmc/), 
  a particle-resolved Monte-Carlo code for atmospheric aerosol simulation
  implemented in Fortran.

## TL;DR (try in a Jupyter notebook)

```python
! pip install git+https://github.com/slayoo/PyPartMC.git
import PyPartMC
```

## Features

- works on Linux, macOS and Windows (compatibility assured with [CI builds](https://github.com/slayoo/PyPartMC/blob/main/.github/workflows/tests.yml))
- hassle-free installation using `pip` (prior PartMC installation **not needed**)
- works out of the box on [mybinder.org](https://mybinder.org/), [Google Colab](colab.research.google.com/) and alike
- support for [de]serialization of all exposed structures using JSON 
- extensive [unit test coverage](https://github.com/slayoo/PyPartMC/tree/main/tests)
- does not use or require shell or netCDF Fortran library
- based on [unmodified PartMC code](https://github.com/slayoo/PyPartMC/tree/main/gitmodules)
- Pythonic API (but retaining PartMC jargon), auto-generated [docs on the web](https://slayoo.github.io/PyPartMC/)

## Usage examples

#### hello world

```python
import PyPartMC
... TODO
```

#### Jupyter notebooks with examples

... TODO ... [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/slayoo/PyPartMC/blob/main/examples/hello_world.ipynb)

#### usage in other projects

PyPartMC is also used within the tests of PySDM project ... TODO

## Implementation outline

- PyPartMC is written in C++, Fortran and Python and uses [pybind11](https://pybind11.readthedocs.io/en/stable/) and [CMake](https://cmake.org/).
- JSON support is handled with [nlohmann::json](https://github.com/nlohmann/json) and [pybind11_json](https://github.com/pybind/pybind11_json)
- PartMC and selected parts of SUNDIALS are compiled during `pip install` and statically linked
- C (SUNDIALS), C++ (pybind11, ...) and Fortran (PartMC) dependencies are linked through [git submodules](https://github.com/slayoo/PyPartMC/blob/main/.gitmodules)
- a [mock of Fortran netCDF API](https://github.com/slayoo/PyPartMC/blob/main/src/fake_netcdf.F90) is used for i/o from/to NumPy using existing netCDF API calls within PartMC
- a [mock of Fortran MPI API](https://github.com/slayoo/PyPartMC/blob/main/src/fake_mpi.F90) is used for error handling using existing MPI API calls within PartMC

## Troubleshooting 

```sh
git clone --recursive git+https://github.com/slayoo/PyPartMC.git
cd PyPartMC
DEBUG=1 pip --verbose install -e .
gdb python 
(gdb) run
```

## Credits

#### PyPartMC:

author: [Sylwester Arabas](https://www.ii.uj.edu.pl/~arabas/)   
funding: [US Department of Energy Atmospheric System Research programme](https://asr.science.energy.gov/)   
copyright: [University of Illinois at Urbana-Champaign](https://atmos.illinois.edu/)   
licence: [GPL v3](https://www.gnu.org/licenses/gpl-3.0.en.html)

#### PartMC:
authors: [Nicole Riemer](https://www.atmos.uiuc.edu/~nriemer/), [Matthew West](https://lagrange.mechse.illinois.edu/mwest/), [Jeff Curtis](https://publish.illinois.edu/jcurtis2/) et al.   
licence: [GPL v2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html) or later

