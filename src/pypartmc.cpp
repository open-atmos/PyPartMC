/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

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

    m.def("run_part", &run_part, R"pbdoc(
        TODO

        TODO
    )pbdoc");

    py::class_<AeroData>(m, "AeroData")
        .def(py::init<>())
    ;

    py::class_<AeroState>(m, "AeroState")
        .def(py::init<>())
    ;

    py::class_<GasData>(m, "GasData")
        .def(py::init<>())
    ;

    py::class_<EnvState>(m, "EnvState")
        .def(py::init<>())
    ;

    py::class_<Scenario>(m, "Scenario")
        .def(py::init<>())
    ;

    py::class_<GasState>(m, "GasState")
        .def(py::init<const std::string&>(), "TODO", py::arg("json") = "")
        .def("__setitem__", GasState::set_item)
        .def("__setitem__", GasState::set_items)
        .def("__getitem__", GasState::get_item)
        .def("__getitem__", GasState::get_items)
        .def("__len__", GasState::__len__)
        .def("__str__", GasState::__str__)
    ;

    py::class_<RunPartOpt>(m, "RunPartOpt")
        .def(py::init<>())
        //.def_readwrite("t_max", &run_part_opt_t::t_max)
        //.def_readwrite("t_output", &run_part_opt_t::t_output)
    ;

    //  TODO: auto util = m.def_submodule("util", "TODO");
    m.def("pow2_above", &pow2_above, py::return_value_policy::copy, R"pbdoc(
        TODO

        TODO
    )pbdoc");

    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
}
