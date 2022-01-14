#define ERROR_MESSAGE_COLLECTION
#include "demo/demo-lib/src/demo_lib.h"
#include "demo/demo-lib/src/demo_lib_errors.h"
#include "error-reporting-cpp/src/error_message.h"
#include <iostream>

int main(int argc, char* argv[]) {
    demo_errors::DEMO_1.str();
    low_value_div(1,1);
    std::cout << ::error_reporting::g_error_message_container.collect_error_messages_as_json();
    return 0;
}