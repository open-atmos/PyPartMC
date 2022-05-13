/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#include "pybind11/pybind11.h"
#include "nlohmann/json.hpp"
#include "pybind11_json/pybind11_json.hpp"
#include "pybind11/numpy.h"

#include "util.hpp"
#include "run_part.hpp"
#include "run_part_opt.hpp"
#include "aero_data.hpp"
#include "aero_state.hpp"
#include "env_state.hpp"
#include "gas_data.hpp"
#include "gas_state.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(PyPartMC, m) {
    m.doc() = R"pbdoc(
        PyPartMC is a Python interface to PartMC.
    )pbdoc";

    m.def("run_part", &run_part, "Do a particle-resolved Monte Carlo simulation.");

    py::class_<AeroData>(m, "AeroData",
        R"pbdoc(
             Aerosol material properties and associated data.
 
             The data in this structure is constant, as it represents physical
             quantities that cannot change over time.
   
             Each aerosol species is identified by an index <tt>i =
             1,...,aero_data_n_spec(aero_data)</tt>. Then \c name(i) is the name of
             that species, \c density(i) is its density, etc. The ordering of the
             species is arbitrary and should not be relied upon (currently it is the
             order in the species data file). The only exception is that it is
             possible to find out which species is water from the \c i_water
             variable.
  
             The names of the aerosol species are not important to PartMC, as
             only the material properties are used. The names are used for
             input and output, and also for communication with MOSAIC. For the
             MOSAIC interface to work correctly the species must be named the
             same, but without the \c _a suffix.
        )pbdoc"
    )
        .def(py::init<>())
    ;

    py::class_<AeroState>(m, "AeroState",
        R"pbdoc(
             The current collection of aerosol particles.
  
             The particles in \c aero_state_t are stored in a single flat
             array (the \c apa data member), with a sorting into size bins and
             weight groups/classes possibly stored in the \c aero_sorted data
             member (if \c valid_sort is true).
  
             Every time we remove particles we keep track of the particle ID
             and the action performed in the aero_info_array_t structure. This
             is typically cleared each time we output data to disk.
        )pbdoc"
    )
        .def(py::init<>())
    ;

    py::class_<GasData>(m, "GasData",
        R"pbdoc(
            Constant gas data.
    
            Each gas species is identified by an integer \c i between 1 and
            \c gas_data_n_spec(gas_data). Species \c i has name \c gas_data%%name(i).
            The variable gas data describing the current mixing ratios is stored
            in the gas_state_t structure, so the mixing ratio of species \c i
            is gas_state%%mix_rat(i).
        )pbdoc"
    )
        .def(py::init<>())
    ;

    py::class_<EnvState>(m,
        "EnvState",
        R"pbdoc(
            Current environment state.
    
            All quantities are instantaneous, describing the state at a
            particular instant of time. Constant data and other data not
            associated with the current environment state is stored in
            scenario_t.
        )pbdoc"
    )
        .def(py::init<>())
    ;

    py::class_<Scenario>(m,
        "Scenario",
        R"pbdoc(
            This is everything needed to drive the scenario being simulated.
  
            The temperature, pressure, emissions and background states are profiles
            prescribed as functions of time by giving a number of times and
            the corresponding data. Simple data such as temperature and pressure is
            linearly interpolated between times, with constant interpolation
            outside of the range of times. Gases and aerosols are
            interpolated with gas_state_interp_1d() and
            aero_dist_interp_1d(), respectively.
        )pbdoc"
    )
        .def(py::init<>())
    ;

    py::class_<GasState>(m,
        "GasState",
        R"pbdoc(
            Current state of the gas mixing ratios in the system.
  
            The gas species are defined by the gas_data_t structure, so that
            \c gas_state%%mix_rat(i) is the current mixing ratio of the gas
            with name \c gas_data%%name(i), etc.
    
            By convention, if gas_state_is_allocated() return \c .false.,
            then the gas_state is treated as zero for all operations on
            it. This will be the case for new \c gas_state_t structures.
        )pbdoc"
    )
        .def(py::init<const nlohmann::json&>(),
            "instantiates and initializes from JSON string", py::arg(0) = py::dict())
        .def("__setitem__", GasState::set_item)
        .def("__setitem__", GasState::set_items)
        .def("__getitem__", GasState::get_item)
        .def("__getitem__", GasState::get_items)
        .def("__len__", GasState::__len__)
        .def("__str__", GasState::__str__,
            "returns a string with JSON representation of the object")
    ;

    py::class_<RunPartOpt>(m, 
        "RunPartOpt",
        "Options controlling the execution of run_part()."
    )
        .def(py::init<>())
        //.def_readwrite("t_max", &run_part_opt_t::t_max)
        //.def_readwrite("t_output", &run_part_opt_t::t_output)
    ;

    //  TODO: auto util = m.def_submodule("util", "TODO");
    m.def(
        "pow2_above", &pow2_above, py::return_value_policy::copy,
        "Return the least power-of-2 that is at least equal to n."
    );

    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
}
