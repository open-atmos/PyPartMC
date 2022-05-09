#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "util.hpp"
#include "run_part.hpp"
#include "run_part_opt.hpp"
#include "aero_data.hpp"
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

    py::class_<aero_data_t>(m, "AeroData")
        .def(py::init<>())
    ;

    py::class_<gas_data_t>(m, "GasData")
        .def(py::init<>())
    ;

    py::class_<gas_state_t>(m, "GasState")
        .def(py::init<const int&>())
        .def("__setitem__", gas_state_t::set_item<py::slice, py::array_t<double>>)
        .def("__getitem__", gas_state_t::get_item<py::slice, py::array_t<double>>)
    ;

    py::class_<run_part_opt_t>(m, "RunPartOpt")
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
