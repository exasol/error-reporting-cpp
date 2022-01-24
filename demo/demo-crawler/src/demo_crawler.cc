#define MAIN_ERROR_REPORTING_CPP
#include "error-reporting-cpp/src/error_message.h"

#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << ::error_reporting::g_error_message_container.collect_error_messages_as_json();
    return 0;
}
