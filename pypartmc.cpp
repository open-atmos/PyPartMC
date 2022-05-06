#include <pybind11/pybind11.h>

extern "C" void fortransub();

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

int run_part(int i, int j) {
    fortransub();
    return i + j;
}

struct run_part_opt_t {
  double t_max;
};

namespace py = pybind11;

PYBIND11_MODULE(PyPartMC, m) {
    m.doc() = R"pbdoc(
        PyPartMC
        --------

        .. currentmodule:: PyPArtMC

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
    ;

    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
}
