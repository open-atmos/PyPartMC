# PyPartMC

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

PyPartMC is a Python interface to [PartMC](https://lagrange.mechse.illinois.edu/partmc/), 
  a particle-resolved Monte-Carlo code for atmospheric aerosol simulation
  implemented in Fortran.


## Features

- works on Linux, macOS and Windows (compatibility assured with CI builds)
- hassle-free installation using `pip` (prior PartMC installation **not needed**)
- works out of the box on Google Colab
- does not use or require shell or netCDF Fortran library
- based on unmodified PartMC code
- extensive unit test coverage
- ...

## Installation

Typically this should be enough (if using Jupyter, prepend an exclamation mark, i.e. `!pip install ...`):

```sh
pip install git+https://github.com/slayoo/PyPartMC.git
```

If trying to figure out what's going wrong:

```sh
git clone git+https://github.com/slayoo/PyPartMC.git
cd PyPartMC
DEBUG=1 pip --verbose install -e .
```

## Usage examples

```python
import PyPartMC


```

... [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/slayoo/PyPartMC/blob/main/hello_world.ipynb)

PyPartMC is also used within the tests of PySDM project ...

## Implementation outline

- PyPartMC is written in C++, Fortran and Python and uses [pybind11](https://pybind11.readthedocs.io/en/stable/) and [CMake](https://cmake.org/).
- unmodified PartMC code is used and is linked through a git submodule
- a mock of Fortran netCDF API is used for i/o from/to NumPy using existing netCDF API calls within PartMC
- a mock of Fortran MPI API is used for error handling using existing MPI API calls within PartMC

## Credits

#### PyPartMC:

author: [Sylwester Arabas](https://www.ii.uj.edu.pl/~arabas/)   
funding: [US Department of Energy Atmospheric System Research programme](https://asr.science.energy.gov/)   
copyright: [University of Illinois at Urbana-Champaign](https://atmos.illinois.edu/)   
licence: [GPL v3](https://www.gnu.org/licenses/gpl-3.0.en.html)

#### PartMC:
authors: [Nicole Riemer](https://www.atmos.uiuc.edu/~nriemer/), [Matthew West](https://lagrange.mechse.illinois.edu/mwest/), [Jeff Curtis](https://publish.illinois.edu/jcurtis2/) et al.   
licence: GPL v2 or later

