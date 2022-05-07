#pragma once

extern "C" void py_pow2_above(int*, int*);

auto pow2_above(int n) {
    int res;
    py_pow2_above(&n, &res);
    return res;
}

extern "C" double py_deg2rad(double);

