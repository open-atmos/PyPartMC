#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "util.hpp"
#include "run_part.hpp"
#include "gas_data.hpp"
#include "gas_state.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(PyPartMC, m) {
    m.doc() = R"pbdoc(
        PyPartMC
        --------

        .. currentmodule:: PyPartMC

        .. autosummary::
           :toctree: _generate

           run_part
    )pbdoc";

    m.def("run_part", &run_part, R"pbdoc(
        TODO

        TODO
    )pbdoc");

    py::class_<gas_data_t>(m, "gas_data_t")
        .def(py::init<>())
    ;

    py::class_<gas_state_t>(m, "gas_state_t")
        .def(py::init<const int&>())
        .def("__setitem__", gas_state_t::set_item<py::slice, py::array_t<double>>)
        .def("__getitem__", gas_state_t::get_item<py::slice, py::array_t<double>>)
    ;

    py::class_<run_part_opt_t>(m, "run_part_opt_t")
        .def(py::init<>())
        .def_readwrite("t_max", &run_part_opt_t::t_max)
        .def_readwrite("t_output", &run_part_opt_t::t_output)
    ;

    //  TODO: auto util = m.def_submodule("util", "TODO");
    m.def("pow2_above", &pow2_above, py::return_value_policy::copy, R"pbdoc(
        TODO

        TODO
    )pbdoc");

    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
}
