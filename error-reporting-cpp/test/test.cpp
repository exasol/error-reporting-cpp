
#define MAIN_ERROR_REPORTING_CPP
#include "error-reporting-cpp/src/error_message.h"

#include "include/gtest/gtest.h"
#include <string>


using namespace error_reporting;


DECLARE_ERR_MSG(EC_DB_1, "Out of memory", {"Buy more RAM"});


DECLARE_ERR_MSG_PARAMS(EC_DB_2, "hd error {{Dummy1}} , {{Dummy2}}",
                       MITIGATIONS("Buy new disk"),PARAMS("Description for Dummy1", "Description for Dummy2"));

TEST(ErrorMessage, BasicErrorMessage){
    const std::string res = g_error_message_container.collect_error_messages_as_json();
    EXPECT_EQ("[{ \"code\": \"EC_DB_1\" ,  \"message\": \"Out of memory\", \"parameters\" : [], \"mitigations\" : [\"Buy more RAM\",]},{ \"code\": \"EC_DB_2\" ,  \"message\": \"hd error {{Dummy1}} , {{Dummy2}}\", \"parameters\" : [\"Description for Dummy1\",\"Description for Dummy2\",], \"mitigations\" : [\"Buy new disk\",]},]", res);
}

TEST(ErrorMessage, BuildSimpleErrorMessage){
    const std::string res = EC_DB_1.str();
    EXPECT_EQ("[EC_DB_1] - Out of memory", res);
}

TEST(ErrorMessage, BuildParametrizedErrorMessage){
    const std::string res = EC_DB_2.build().param("Dummy1", 1).param("Dummy2", "bla").str();
    EXPECT_EQ("[EC_DB_2] : hd error 1 , bla", res);
}