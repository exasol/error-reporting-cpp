#define MAIN_ERROR_REPORTING_CPP
#include "demo/macro/demo1-lib/src/demo_lib.h"
#include "demo/macro/demo2-lib/src/demo_lib.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Result of log(4) is" << my_log(4) << std::endl;
    std::cout << "Result of low_value_div(40/8) is" << low_value_div(40, 8) << std::endl;
    std::cout << "Result of low_value_div(400/8) is" << low_value_div(400, 8) << std::endl;
}
