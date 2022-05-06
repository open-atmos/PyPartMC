#include <pybind11/pybind11.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

struct run_part_opt_t {
  double t_max, t_output;
};

extern "C" void py_run_part(run_part_opt_t);

void run_part(const run_part_opt_t &run_part_opt) {
    py_run_part(run_part_opt);
}

extern "C" int_fast8_t py_pow2_above(int_fast8_t*);

int_fast8_t pow2_above(int_fast8_t n) {
    return py_pow2_above(&n);
}

extern "C" double py_deg2rad(double);

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
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    py::class_<run_part_opt_t>(m, "run_part_opt_t")
        .def(py::init<>())
        .def_readwrite("t_max", &run_part_opt_t::t_max)
        .def_readwrite("t_output", &run_part_opt_t::t_output)
    ;

    auto util = m.def_submodule("util", "TODO");
    util.def("pow2_above", &pow2_above, R"pbdoc(
        TODO

        TODO
    )pbdoc");

    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
}
