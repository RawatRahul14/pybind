#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

namespace py = pybind11;

long long sum_of_squares(long long n) {
    long long total = 0;
    for (long long i = 0; i < n; i++) {
        total += i * i;
    }
    return total;
}

PYBIND11_MODULE(speed_test, m) {
    m.doc() = "C++ speed test module";
    m.def("sum_of_squares", &sum_of_squares, "Compute sum of squares from 0 to n-1");
}