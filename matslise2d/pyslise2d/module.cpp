#include "module.h"

void pyslise2d(py::module &);

void pyslise3d(py::module &);

PYBIND11_MODULE(pyslise2d, m) {
    pyslise2d(m);

    pyslise3d(m);
}
