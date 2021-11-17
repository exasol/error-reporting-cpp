#include "include/gtest/gtest.h"
#include <string>

#define MAIN_ERROR_REPORTING_CPP
#include "error-reporting-cpp/src/error_message.h"


using namespace error_reporting;
#define TEST_(...) { __VA_ARGS__ }
DECLARE_ERR_MSG(EC_DB_1, "Out of memory", {"Buy more RAM"});
DECLARE_ERR_MSG_PARAMS(EC_DB_2, "hd error", {"Buy new disk"},TEST_("Dummy1", "Dummy2"));
//error_message_declaration g_db_1_error("EC_DB_1", "Out of memory", {"Buy more RAM"});
//error_message_declaration_with_param<2> g_db_2_error("EC_DB_2", "hd error",
//                                                     {"Buy new disk"}, {"Dummy1", "Dummy2"});

TEST(ErrorMessage, BasicErrorMessage){
    const std::string res = g_error_message_container.collect_error_messages_as_json();
    EXPECT_EQ("[{ \"code\": \"EC_DB_1\" ,  \"message\": \"Out of memory\", \"parameters\" : [], \"mitigations\" : [\"Buy more RAM\",]},{ \"code\": \"EC_DB_2\" ,  \"message\": \"hd error\", \"parameters\" : [\"Dummy1\",\"Dummy2\",], \"mitigations\" : [\"Buy new disk\",]},]", res);
}

TEST(ErrorMessage, BuildSimpleErrorMessage){
    const std::string res = g_EC_DB_1.str();
    EXPECT_EQ("[EC_DB_1] - Out of memory", res);
}

TEST(ErrorMessage, BuildParametrizedErrorMessage){
    const std::string res = g_db_2_error.build().param(1).param("blabla").str();
    EXPECT_EQ("[EC_DB_2] - hd error: (1,blabla)", res);
}