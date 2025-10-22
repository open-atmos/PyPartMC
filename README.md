![logo](https://raw.githubusercontent.com/wiki/open-atmos/PyPartMC/logo.svg)

# PyPartMC

PyPartMC is a Python interface to [PartMC](https://lagrange.mechse.illinois.edu/partmc/), 
  a particle-resolved Monte-Carlo code for atmospheric aerosol simulation.
Development of PyPartMC has been intended to remove limitations to the use of Fortran-implemented PartMC.
PyPartMC facilitates the dissemination of computational research results by streamlining independent execution 
  of PartMC simulations (also during peer-review processes).
Additionally, the ability to easily package examples, simple simulations, and results in a web-based notebook 
  allows PyPartMC to support the efforts of many members of the scientific community, including researchers,
  instructors, and students, with nominal software and hardware requirements.

Documentation of PyPartMC is hosted at https://open-atmos.github.io/PyPartMC.
PyPartMC is implemented in C++ and it also constitutes a C++ API to the PartMC Fortran internals.
The Python API can facilitate using PartMC from other environments - see, e.g., Julia and Matlab examples below.

For an outline of the project, rationale, architecture, and features, refer to: [D'Aquino et al., 2024 (SoftwareX)](https://doi.org/10.1016/j.softx.2023.101613) (please cite if PyPartMC is used in your research).
For a list of talks and other relevant resources, please see [project Wiki](https://github.com/open-atmos/PyPartMC/wiki/).
If interested in contributing to PyPartMC, please have a look a the [notes for developers](https://github.com/open-atmos/PyPartMC/tree/main/CONTRIBUTING.md).

[![US Funding](https://img.shields.io/static/v1?label=US%20DOE%20Funding%20by&color=267c32&message=ASR&logoWidth=25&logo=image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAB4AAAAQCAMAAAA25D/gAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAASFBMVEVOTXyyIjRDQnNZWINZWITtzdFUU4BVVIFVVYHWiZM9PG/KZnNXVoJaWYT67/FKSXhgX4hgX4lcW4VbWoX03uHQeIN2VXj///9pZChlAAAAAWJLR0QXC9aYjwAAAAd0SU1FB+EICRMGJV+KCCQAAABdSURBVBjThdBJDoAgEETRkkkZBBX0/kd11QTTpH1/STqpAAwWBkobSlkGbt0o5xmEfqxDZJB2Q6XMoBwnVSbTylWp0hi42rmbwTOYPDfR5Kc+07IIUQQvghX9THsBHcES8/SiF0kAAAAldEVYdGRhdGU6Y3JlYXRlADIwMTctMDgtMDlUMTk6MDY6MzcrMDA6MDCX1tBgAAAAJXRFWHRkYXRlOm1vZGlmeQAyMDE3LTA4LTA5VDE5OjA2OjM3KzAwOjAw5oto3AAAAABJRU5ErkJggg==)](https://asr.science.energy.gov/) [![PL Funding](https://img.shields.io/static/v1?label=PL%20Funding%20by&color=d21132&message=NCN&logoWidth=25&logo=image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAANCAYAAACpUE5eAAAABmJLR0QA/wD/AP+gvaeTAAAAKUlEQVQ4jWP8////fwYqAiZqGjZqIHUAy4dJS6lqIOMdEZvRZDPcDQQAb3cIaY1Sbi4AAAAASUVORK5CYII=)](https://www.ncn.gov.pl/?language=en)
   
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![Copyright](https://img.shields.io/static/v1?label=Copyright&color=249fe2&message=UIUC&)](https://atmos.illinois.edu/)
[![tests+pypi](https://github.com/open-atmos/PyPartMC/actions/workflows/buildwheels.yml/badge.svg)](https://github.com/open-atmos/PyPartMC/actions/workflows/buildwheels.yml)
[![API docs](https://shields.mitmproxy.org/badge/docs-pdoc.dev-brightgreen.svg)](https://open-atmos.github.io/PyPartMC/)
[![codecov](https://codecov.io/gh/open-atmos/PyPartMC/graph/badge.svg?token=27IK9ZIQXE)](https://codecov.io/gh/open-atmos/PyPartMC)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.7662635.svg)](https://doi.org/10.5281/zenodo.7662635)
[![PyPI version](https://badge.fury.io/py/PyPartMC.svg)](https://pypi.org/p/PyPartMC)
[![Project Status: Active – The project has reached a stable, usable state and is being actively developed.](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)
[![pyOpenSci Peer-Reviewed](https://pyopensci.org/badges/peer-reviewed.svg)](https://github.com/pyOpenSci/software-review/issues/179)

[![Python 3](https://img.shields.io/static/v1?label=Python&logo=Python&color=3776AB&message=3)](https://www.python.org/)
[![Linux OK](https://img.shields.io/static/v1?label=Linux&logo=Linux&color=yellow&message=%E2%9C%93)](https://en.wikipedia.org/wiki/Linux)
[![macOS OK](https://img.shields.io/static/v1?label=macOS&logo=Apple&color=silver&message=%E2%9C%93)](https://en.wikipedia.org/wiki/macOS)
[![Windows OK](https://img.shields.io/static/v1?label=Windows&logo=Windows&color=white&message=%E2%9C%93)](https://en.wikipedia.org/wiki/Windows)
[![Jupyter](https://img.shields.io/static/v1?label=Jupyter&logo=Jupyter&color=f37626&message=%E2%9C%93)](https://jupyter.org/)

## Installation 

### Using the command-line `pip` tool (also applies to conda environments)
```bash
pip install PyPartMC
```

Note that, depending on the environment (OS, hardware, Python version), the pip-install invocation 
  may either trigger a download of a pre-compiled binary, or trigger compilation of PyPartMC.
In the latter case, a Fortran compiler and some development tools includiong CMake, m4 and perl
  are required (while all non-Python dependencies are included in the PyPartMC source archive).
In both cases, all Python dependencies will be resolved by pip.

### In a Jupyter notebook cell (also on Colab or jupyter-hub instances)

```python
! pip install PyPartMC
import PyPartMC
```

#### Jupyter notebooks with examples 
Note: clicking the badges below redirects to cloud-computing platforms. The mybinder.org links allow anonymous execution, Google Colab requires logging in with a Google account, ARM JupyerHub requires logging in with an ARM account (and directing Jupyter to a particular notebook within the `examples` folder).

The example notebooks feature additional dependencies that can be installed with:
```bash
pip install PyPartMC[examples]
```

- Urban plume scenario demo (as in [PartMC](https://github.com/compdyn/partmc/tree/master/scenarios/1_urban_plume)):    
[![View notebook](https://img.shields.io/static/v1?label=render%20on&logo=github&color=87ce3e&message=GitHub)](https://github.com/open-atmos/PyPartMC/blob/main/examples/particle_simulation.ipynb) 
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/open-atmos/PyPartMC/blob/main/examples/particle_simulation.ipynb) 
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/open-atmos/PyPartMC.git/main?urlpath=lab/tree/examples/particle_simulation.ipynb)
[![ARM JupyterHub](https://img.shields.io/static/v1?label=launch%20in&logo=jupyter&color=lightblue&message=ARM+JupyterHub)](https://jupyterhub.arm.gov/hub/user-redirect/git-pull?repo=https%3A//github.com/open-atmos/PyPartMC&branch=main&urlPath=)   
- Chamber simulation example from Barrel Study ([Tian el al., 2017](https://doi.org/10.1080/02786826.2017.1311988)):    
[![View notebook](https://img.shields.io/static/v1?label=render%20on&logo=github&color=87ce3e&message=GitHub)](https://github.com/open-atmos/PyPartMC/blob/main/examples/chamber.ipynb) 
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/open-atmos/PyPartMC/blob/main/examples/chamber.ipynb) 
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/open-atmos/PyPartMC.git/main?urlpath=lab/tree/examples/chamber.ipynb)
[![ARM JupyterHub](https://img.shields.io/static/v1?label=launch%20in&logo=jupyter&color=lightblue&message=ARM+JupyterHub)](https://jupyterhub.arm.gov/hub/user-redirect/git-pull?repo=https%3A//github.com/open-atmos/PyPartMC&branch=main&urlPath=)   
- Dry-Wet Particle Size Equilibration with PartMC and PySDM:   
[![View notebook](https://img.shields.io/static/v1?label=render%20on&logo=github&color=87ce3e&message=GitHub)](https://github.com/open-atmos/PyPartMC/blob/main/examples/lognorm_ex.ipynb) 
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/open-atmos/PyPartMC/blob/main/examples/lognorm_ex.ipynb)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/open-atmos/PyPartMC.git/main?urlpath=lab/tree/examples/lognorm_ex.ipynb)
[![ARM JupyterHub](https://img.shields.io/static/v1?label=launch%20in&logo=jupyter&color=lightblue&message=ARM+JupyterHub)](https://jupyterhub.arm.gov/hub/user-redirect/git-pull?repo=https%3A//github.com/open-atmos/PyPartMC&branch=main&urlPath=)
[![Voila](https://img.shields.io/static/v1?label=Voil%C3%A0&logo=jupyter&color=teal&message=web+app)](https://mybinder.org/v2/gh/open-atmos/PyPartMC/main?urlpath=voila%2Frender%2Fexamples%2Flognorm_ex.ipynb)
- Simulation output processing example (loading from netCDF files using PyPartMC):    
[![View notebook](https://img.shields.io/static/v1?label=render%20on&logo=github&color=87ce3e&message=GitHub)](https://github.com/open-atmos/PyPartMC/blob/main/examples/process_simulation_output.ipynb)
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/open-atmos/PyPartMC/blob/main/examples/process_simulation_output.ipynb)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/open-atmos/PyPartMC.git/main?urlpath=lab/tree/examples/process_simulation_output.ipynb)
[![ARM JupyterHub](https://img.shields.io/static/v1?label=launch%20in&logo=jupyter&color=lightblue&message=ARM+JupyterHub)](https://jupyterhub.arm.gov/hub/user-redirect/git-pull?repo=https%3A//github.com/open-atmos/PyPartMC&branch=main&urlPath=)
- Optical properties calculation using external Python package ([PyMieScatt](https://pymiescatt.readthedocs.io/en/latest/)):       
[![View notebook](https://img.shields.io/static/v1?label=render%20on&logo=github&color=87ce3e&message=GitHub)](https://github.com/open-atmos/PyPartMC/blob/main/examples/mie_optical.ipynb)
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/open-atmos/PyPartMC/blob/main/examples/mie_optical.ipynb)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/open-atmos/PyPartMC.git/main?urlpath=lab/tree/examples/mie_optical.ipynb)
[![ARM JupyterHub](https://img.shields.io/static/v1?label=launch%20in&logo=jupyter&color=lightblue&message=ARM+JupyterHub)](https://jupyterhub.arm.gov/hub/user-redirect/git-pull?repo=https%3A//github.com/open-atmos/PyPartMC&branch=main&urlPath=)
- Cloud parcel example featuring supersaturation-evolution-coupled CCN activation and drop growth:    
[![View notebook](https://img.shields.io/static/v1?label=render%20on&logo=github&color=87ce3e&message=GitHub)](https://github.com/open-atmos/PyPartMC/blob/main/examples/cloud_parcel.ipynb)
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/open-atmos/PyPartMC/blob/main/examples/cloud_parcel.ipynb)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/open-atmos/PyPartMC.git/main?urlpath=lab/tree/examples/cloud_parcel.ipynb)
[![ARM JupyterHub](https://img.shields.io/static/v1?label=launch%20in&logo=jupyter&color=lightblue&message=ARM+JupyterHub)](https://jupyterhub.arm.gov/hub/user-redirect/git-pull?repo=https%3A//github.com/open-atmos/PyPartMC&branch=main&urlPath=)
- Immersion freezing example:   
[![View notebook](https://img.shields.io/static/v1?label=render%20on&logo=github&color=87ce3e&message=GitHub)](https://github.com/open-atmos/PyPartMC/blob/main/examples/immersion_freezing.ipynb)
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/open-atmos/PyPartMC/blob/main/examples/immersion_freezing.ipynb)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/open-atmos/PyPartMC.git/main?urlpath=lab/tree/examples/immersion_freezing.ipynb)
[![ARM JupyterHub](https://img.shields.io/static/v1?label=launch%20in&logo=jupyter&color=lightblue&message=ARM+JupyterHub)](https://jupyterhub.arm.gov/hub/user-redirect/git-pull?repo=https%3A//github.com/open-atmos/PyPartMC&branch=main&urlPath=)
- Coagulation model intercomparison for additive (Golovin) kernel with: PyPartMC, [PySDM](https://open-atmos.github.io/PySDM), [Droplets.jl](https://github.com/emmacware/droplets.jl) and [dustpy](https://stammler.github.io/dustpy/):    
[![View notebook](https://img.shields.io/static/v1?label=render%20on&logo=github&color=87ce3e&message=GitHub)](https://github.com/open-atmos/PyPartMC/blob/main/examples/additive_coag_comparison.ipynb) 
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/open-atmos/PyPartMC/blob/main/examples/additive_coag_comparison.ipynb) 
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/open-atmos/PyPartMC.git/main?urlpath=lab/tree/examples/additive_coag_comparison.ipynb)
[![ARM JupyterHub](https://img.shields.io/static/v1?label=launch%20in&logo=jupyter&color=lightblue&message=ARM+JupyterHub)](https://jupyterhub.arm.gov/hub/user-redirect/git-pull?repo=https%3A//github.com/open-atmos/PyPartMC&branch=main&urlPath=)   
- Particle simulation with multiphase chemistry handled using [CAMP](https://doi.org/10.5194/gmd-15-3663-2022) (without coagulation):   
[![View notebook](https://img.shields.io/static/v1?label=render%20on&logo=github&color=87ce3e&message=GitHub)](https://github.com/open-atmos/PyPartMC/blob/main/examples/particle_simulation_with_camp.ipynb.ipynb) 
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/open-atmos/PyPartMC/blob/main/examples/particle_simulation_with_camp.ipynb) 
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/open-atmos/PyPartMC.git/main?urlpath=lab/tree/examples/particle_simulation_with_camp.ipynb)
[![ARM JupyterHub](https://img.shields.io/static/v1?label=launch%20in&logo=jupyter&color=lightblue&message=ARM+JupyterHub)](https://jupyterhub.arm.gov/hub/user-redirect/git-pull?repo=https%3A//github.com/open-atmos/PyPartMC&branch=main&urlPath=)   

## Features

- works on Linux, macOS and Windows (compatibility assured with [CI builds](https://github.com/open-atmos/PyPartMC/blob/main/.github/workflows/buildwheels.yml))
- hassle-free installation using `pip` (prior PartMC installation **not needed**)
- works out of the box on [mybinder.org](https://mybinder.org/), [Google Colab](colab.research.google.com/) and alike
- ships with [a set of examples](https://github.com/open-atmos/PyPartMC/tree/main/examples) maintained in a form of Jupyter notebooks
- Pythonic API (but retaining PartMC jargon) incl. Python GC deallocation of Fortran objects
- specification of parameters using native Python datatypes (lists, dicts) in place of PartMC spec files
- code snippets in README depicting how to use PyPartMC from Julia and Matlab (also executed on CI)
- auto-generated [API docs on the web](https://open-atmos.github.io/PyPartMC/)
- support for [de]serialization of selected wrapped structures using JSON 
- based on [unmodified PartMC code](https://github.com/open-atmos/PyPartMC/tree/main/gitmodules)
- does not use or require shell or any pre-installed libraries
- aiming at 100% [unit test coverage](https://github.com/open-atmos/PyPartMC/tree/main/tests)

## Usage examples

The listings below depict how the identical task of randomly sampling particles from an aerosol size distribution in PartMC can be
done in different programming languages.

For a Fortran equivalent of the Python, Julia and Matlab programs below, see the [`readme_fortran` folder](https://github.com/open-atmos/PyPartMC/tree/main/readme_fortran).

#### Python

```Python
import numpy as np

import PyPartMC as ppmc
from PyPartMC import si

aero_data = ppmc.AeroData((
    #      [density, ions in solution, molecular weight, kappa, abifm_m, abifm_c]
    {"OC": [1000 *si.kg/si.m**3, 0, 1e-3 *si.kg/si.mol, 0.001, 0, 0]},
    {"BC": [1800 *si.kg/si.m**3, 0, 1e-3 *si.kg/si.mol, 0, 0 , 0]},
))

aero_dist = ppmc.AeroDist(
    aero_data,
    [{
        "cooking": {
            "mass_frac": [{"OC": [1]}],
            "diam_type": "geometric",
            "mode_type": "log_normal",
            "num_conc": 3200 / si.cm**3,
            "geom_mean_diam": 8.64 * si.nm,
            "log10_geom_std_dev": 0.28,
        },
        "diesel": {
            "mass_frac": [{"OC": [0.3]}, {"BC": [0.7]}],
            "diam_type": "geometric",
            "mode_type": "log_normal",
            "num_conc": 2900 / si.cm**3,
            "geom_mean_diam": 50 * si.nm,
            "log10_geom_std_dev": 0.24,
        }
    }],
)

n_part = 100
aero_state = ppmc.AeroState(aero_data, n_part, "nummass_source")
aero_state.dist_sample(aero_dist)
print(np.dot(aero_state.masses(), aero_state.num_concs), "# kg/m3")
```

#### Julia (using [PyCall.jl](https://github.com/JuliaPy/PyCall.jl))
```Julia
using Pkg
Pkg.add("PyCall")

using PyCall
ppmc = pyimport("PyPartMC")
si = ppmc["si"]

aero_data = ppmc.AeroData((
  #       (density, ions in solution, molecular weight, kappa, abifm_m, abifm_c)
  Dict("OC"=>(1000 * si.kg/si.m^3, 0, 1e-3 * si.kg/si.mol, 0.001, 0, 0)),
  Dict("BC"=>(1800 * si.kg/si.m^3, 0, 1e-3 * si.kg/si.mol, 0, 0, 0))
))

aero_dist = ppmc.AeroDist(aero_data, (
  Dict( 
    "cooking" => Dict(
      "mass_frac" => (Dict("OC" => (1,)),),
      "diam_type" => "geometric",
      "mode_type" => "log_normal",
      "num_conc" => 3200 / si.cm^3,
      "geom_mean_diam" => 8.64 * si.nm,
      "log10_geom_std_dev" => .28,
    ),
    "diesel" => Dict(
      "mass_frac" => (Dict("OC" => (.3,)), Dict("BC" => (.7,))),
      "diam_type" => "geometric",
      "mode_type" => "log_normal",
      "num_conc" => 2900 / si.cm^3,
      "geom_mean_diam" => 50 * si.nm,
      "log10_geom_std_dev" => .24,
    )
  ),
))

n_part = 100
aero_state = ppmc.AeroState(aero_data, n_part, "nummass_source")
aero_state.dist_sample(aero_dist)
print(aero_state.masses()'aero_state.num_concs, "# kg/m3")
```

#### Matlab (using [Matlab's built-in Python interface](https://www.mathworks.com/help/matlab/python-language.html))

notes (see the [PyPartMC Matlab CI workflow](https://github.com/open-atmos/PyPartMC/blob/main/.github/workflows/readme_listings.yml) for an example on how to achieve it on Ubuntu 20):
- Matlab ships with convenience copies of C, C++ and Fortran runtime libraries which are `dlopened()` by default; one way to make PyPartMC OK with it is to [pip-]install by compiling from source using the very same version of GCC that Matlab borrowed these libraries from (e.g., [GCC 9 for Matlab R2022a, etc](https://www.mathworks.com/support/requirements/supported-compilers-linux.html));
- Matlab needs to [use the same Python interpretter/venv](https://www.mathworks.com/support/requirements/python-compatibility.html) as the pip invocation used to install PyPartMC;

````Matlab
ppmc = py.importlib.import_module('PyPartMC');
si = py.importlib.import_module('PyPartMC').si;

aero_data = ppmc.AeroData(py.tuple({ ...
  py.dict(pyargs("OC", py.tuple({1000 * si.kg/si.m^3, 0, 1e-3 * si.kg/si.mol, 0.001, 0, 0}))), ...
  py.dict(pyargs("BC", py.tuple({1800 * si.kg/si.m^3, 0, 1e-3 * si.kg/si.mol, 0, 0, 0}))) ...
}));

aero_dist = ppmc.AeroDist(aero_data, py.tuple({ ...
  py.dict(pyargs( ...
    "cooking", py.dict(pyargs( ...
      "mass_frac", py.tuple({py.dict(pyargs("OC", py.tuple({1})))}), ...
      "diam_type", "geometric", ...
      "mode_type", "log_normal", ...
      "num_conc", 3200 / si.cm^3, ...
      "geom_mean_diam", 8.64 * si.nm, ...
      "log10_geom_std_dev", .28 ...
    )), ...
    "diesel", py.dict(pyargs( ...
      "mass_frac", py.tuple({ ...
        py.dict(pyargs("OC", py.tuple({.3}))), ...
        py.dict(pyargs("BC", py.tuple({.7}))), ...
      }), ...
      "diam_type", "geometric", ...
      "mode_type", "log_normal", ...
      "num_conc", 2900 / si.cm^3, ...
      "geom_mean_diam", 50 * si.nm, ...
      "log10_geom_std_dev", .24 ...
    )) ...
  )) ...
}));

n_part = 100;
aero_state = ppmc.AeroState(aero_data, n_part, "nummass_source");
aero_state.dist_sample(aero_dist);
masses = cell(aero_state.masses());
num_concs = cell(aero_state.num_concs);
fprintf('%g # kg/m3\n', dot([masses{:}], [num_concs{:}]))
````

#### usage in other projects

PyPartMC is used within the [test workflow of the PySDM project](https://github.com/atmos-cloud-sim-uj/PySDM/tree/main/tests/smoke_tests/box/partmc).

## Other packages with relevant feature scope

- [aerosolGDEFoam](https://openaerosol.sourceforge.io/): OpenFOAM CFD-coupled aerosol dynamics including nucleation, coagulation, and surface growth 
- [AIOMFAC and AIOMFAC-web](http://web.archive.org/web/20250730204937/https://www.aiomfac.caltech.edu/): Fortran-implemented aerosol thermodynamic model for calculation of activity coefficients in organic-inorganic mixtures – from simple binary solutions to complex multicomponent systems
- [DustPy](https://stammler.github.io/dustpy/): Python package for modelling dust evolution in protoplanetary disks (differences: focus on astrophysical applications vs. atmospheric aerosol)
- [multilayerpy](https://github.com/tintin554/multilayerpy): kinetic multi-layer model for aerosol particles and films 
- [PyBox](https://pybox.readthedocs.io): aerosol simulation model featuring gas and particle chamistry (differences: PyBox focuses on chemical mechanisms; PyPartMC is an interface to PartMC which focuses on physics - e.g., collisions of aerosol particles - while chemical processes are handled with external software, e.g., CAMP or MOSAIC)
- [PyCHAM](https://github.com/simonom/PyCHAM): CHemistry with Aerosol Microphysics in Python Box Model for modelling of indoor environments, including aerosol chambers
- [PySDM](https://open-atmos.github.io/PySDM): particle-based Monte-Carlo aerosol-cloud simulation package (differences: PySDM focuses on growth and breakup processes relevant to cloud droplets; PyPartMC focuses on processes relevant to air pollutants and their chemical and physical transformations)
- [SSH-aerosol](https://github.com/sshaerosol/ssh-aerosol): C++/Fortran package for simulating evolution of primary and secondary atmospheric aerosols

## FAQ
- Q: How to install PyPartMC with MOSAIC enabled?    
  A: Installation can be done using `pip`, however, `pip` needs to be instructed not to use binary packages available at pypi.org but rather to compile from source (pip will download the source from pip.org), and the path to compiled MOSAIC library needs to be provided at compile-time; the following command should convey it:
```bash
MOSAIC_HOME=<<PATH_TO_MOSAIC_LIB>> pip install --force-reinstall --no-binary=PyPartMC PyPartMC
```

- Q: Why `pip install PyPartMC` triggers compilation on my brand new Apple machine, while it quickly downloads and installs binary packages when executed on older Macs, Windows or Linux?    
  A: We are providing binary wheels on PyPI for Apple-silicon (arm64) machines for selected macOS version made available by Github. In case the macOS version you are using is newer, compilation from source is triggered.  

- Q: Why some of the constructors expect data to be passed as **lists of single-entry dictionaries** instead of multi-element dictionaries?    
  A: This is intentional and related with PartMC relying on the order of elements within spec-file input; while Python dictionaries preserve ordering (insertion order), JSON format does not, and we intend to make these data structures safe to be [de]serialized using JSON.   

- Q: How to check the version of PartMC that PyPartMC was compiled against?    
  A: Version numbers of compile-time dependencies of PyPartMC, including PartMC, can be accessed as follows:
```Python
import PyPartMC
PyPartMC.__versions_of_build_time_dependencies__['PartMC']
```

- Q: Why m4 and perl are required at compile time?    
  A: PyPartMC includes parts of netCDF and HDF5 codebases which depend on m4 and perl, respectively, for generating source files before compilation.

## Troubleshooting 

#### Common installation issues 
```
error: [Errno 2] No such file or directory: 'cmake'
```
Try rerunning after installing CMake, e.g., using `apt-get install cmake` (Ubuntu/Debian), `brew install cmake` (homebrew on macOS) or using [MSYS2](https://www.msys2.org/docs/cmake/) on Windows.

```
No CMAKE_Fortran_COMPILER could be found.
```
Try installing a Fortran compiler (e.g., `brew reinstall gcc` with Homebrew on macOS or using [MSYS2](https://packages.msys2.org/package/mingw-w64-x86_64-gcc-fortran?repo=mingw64) on Windows).

```
Could not find NC_M4 using the following names: m4, m4.exe
```
Try installing `m4` (e.g., using [MSYS2](https://packages.msys2.org/package/m4?repo=msys&variant=x86_64) on Windows).

## Acknowledgement and citations

We would greatly appreciate citation of the PartMC model description paper (Riemer et al., 2009) 
and the PyPartMC description paper (D’Aquino et al., 2024) if PyPartMC was used in your study.
The citations are:
- Riemer, N., M. West, R. A. Zaveri, R. C. Easter: Simulating the evolution of soot 
  mixing-state with a particle-resolved aerosol model    
  J. Geophys. Res., 114, D09202, 2009, DOI: [10.1029/2008JD011073](https://doi.org/10.1029/2008JD011073)
- D’Aquino, Z., S. Arabas, J. H. Curtis, A. Vaishnav, N. Riemer, M. West: PyPartMC: A 
  pythonic interfact to a particle-resolved, Monte Carlo aerosol simulation framework   
  SoftwareX, 25, 101613, 2024, DOI: [10.1016/j.softx.2023.101613](https://doi.org/10.1016/j.softx.2023.101613)

The following paragraph provides a more substantial description of PartMC (text released into the public domain and can be freely copied by anyone for any purpose):

> PartMC is a stochastic, particle-resolved aerosol box model. It tracks the 
composition of many computational particles (10<sup>4</sup> to 10<sup>6</sup>) within a well-mixed air 
volume, each represented by a composition vector that evolves based on physical 
and chemical processes. The physical processes—including Brownian coagulation, 
new particle formation, emissions, dilution, and deposition—are simulated using a 
stochastic Monte Carlo approach via a Poisson process while chemical processes are
simulated deterministically for each computational particle. The weighted flow 
algorithm (DeVille, Riemer, and West, 2011, 2019) enhances efficiency and reduces 
ensemble variance. Detailed numerical methods are described in Riemer et al. 
(2009), DeVille et al. (2011, 2019), and Curtis et al. (2016). PartMC is open-source 
under the GNU GPL v2 and available at 
[github.com/compdyn/partmc](https://github.com/compdyn/partmc).
>
> References:
> - Curtis, J. H., M. D. Michelotti, N. Riemer, M. T. Heath, M. West: Accelerated 
simulation of stochastic particle removal processes in particle-resolved aerosol 
models, J. Computational Phys., 322, 21-32, 2016, DOI: [10.1016/j.jcp.2016.06.029](https://doi.org/10.1016/j.jcp.2016.06.029)
> - DeVille, L., N. Riemer, M. West, Convergence of a generalized weighted flow 
algorithm for stochastic particle coagulation, J. Computational Dynamics, 6, 69-94, 
2019, DOI: [10.3934/jcd.2019003](https://doi.org/10.3934/jcd.2019003)
> - DeVille, R. E. L., N. Riemer, M. West, The Weighted Flow Algorithm (WFA) for 
stochastic particle coagulation, J. Computational Phys., 230, 8427-8451, 2011, 
DOI: [10.1016/j.jcp.2011.07.027](https://doi.org/10.1016/j.jcp.2011.07.027)
> - Riemer, N., M. West, R. A. Zaveri, R. C. Easter, Simulating the evolution of soot 
mixing-state with a particle-resolved aerosol model, J. Geophys. Res., 114, D09202, 
2009., DOI: [10.1029/2008JD011073](https://doi.org/10.1029/2008JD011073)

## Credits

#### PyPartMC:

authors: [PyPartMC developers](https://github.com/open-atmos/PyPartMC/graphs/contributors)   
funding: [US Department of Energy Atmospheric System Research programme](https://asr.science.energy.gov/), [Polish National Science Centre](https://ncn.gov.pl/en)   
copyright: [University of Illinois at Urbana-Champaign](https://atmos.illinois.edu/)   
licence: [GPL v3](https://www.gnu.org/licenses/gpl-3.0.en.html)

#### PartMC:
authors: [Nicole Riemer](https://www.atmos.uiuc.edu/~nriemer/), [Matthew West](https://lagrange.mechse.illinois.edu/mwest/), [Jeff Curtis](https://publish.illinois.edu/jcurtis2/) et al.   
licence: [GPL v2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html) or later

