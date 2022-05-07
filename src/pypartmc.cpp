#include <pybind11/pybind11.h>

#include "util.hpp"
#include "run_part.hpp"

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
