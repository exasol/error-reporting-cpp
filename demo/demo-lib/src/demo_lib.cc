#include "demo/demo-lib/src/demo_lib.h"
#include "demo/demo-lib/src/demo_lib_errors.h"
#include <cmath>

int low_value_div(int a, int b) {
    if (0 == b) {
        throw std::runtime_error(demo_errors::DEMO_1.str());
    }
    if (abs(b) > 100) {
        throw std::runtime_error(demo_errors::DEMO_2.build().setFUNC_PARAM("b").setVALUE(b).str());
    }
    if (abs(a) > 100) {
        throw std::runtime_error(demo_errors::DEMO_2.build().setFUNC_PARAM("a").setVALUE(a).str());
    }
    return a/b;
}