#include "include/gtest/gtest.h"
#include <string>

#define MAIN_ERROR_REPORTING_CPP
#include "error-reporting-cpp/src/error_message.h"


using namespace error_reporting;

const std::array<const char*, 2> g_out_of_memory_parameters = {
        "dummy1", "dummy2"
};

const std::array<const char*, 2> g_out_of_memory_error_mitigations = {
        "dummy1", "dummy2"
};

DECLARE_ERROR_CODE(EC_DB_1, "Out of memory", ERR_ARRAY("dummy1", "dummy2"), ERR_ARRAY("Buy more RAM"))

TEST(ErrorMessage, BasicErrorMessage){
    const std:: string res = g_error_message_container.collect_error_messages_as_json();
    EXPECT_EQ("[{ \"code\": \"EC_DB_1\" ,  \"message\": \"Out of memory\", \"parameters\" : [\"Buy more RAM\",], \"mitigations\":[\"dummy1\",\"dummy2\",]},]", res);
}