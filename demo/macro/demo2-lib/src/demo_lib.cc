#include "demo/macro/demo2-lib/src/demo_lib.h"
#include "demo/macro/demo2-lib/src/demo_lib_errors.h"
#include <math.h>

double my_log(double a) {
    if (0.0 >= a) {
        throw std::runtime_error(demo2_errors::DEMO2_1.str());
    }
    return log(a);
}
