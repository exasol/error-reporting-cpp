
#define MAIN_ERROR_REPORTING_CPP
#include "error-reporting-cpp/src/error_message.h"

#include "include/gtest/gtest.h"
#include <string>


using namespace error_reporting;


DECLARE_ERR_MSG(EC_DB_1, "Out of memory.", MITIGATIONS("Buy more RAM"));


DECLARE_ERR_MSG_PARAMS(EC_DB_2, "hd error {{Dummy1}} , {{Dummy2}}.",
                       MITIGATIONS("Replace {{Dummy1}}"), PARAM("Description for Dummy1", Dummy1), PARAM("Description for Dummy2", Dummy2));

TEST(ErrorMessage, BasicErrorMessage){
    const std::string res = g_error_message_container.collect_error_messages_as_json();


    EXPECT_EQ("[{ \"code\": \"\" ,  \"message\": \"hd error {{Dummy1}} , {{Dummy2}}.\", "
              "\"parameters\" : [\"Description for Dummy1\",\"Description for Dummy2\",], "
              "\"mitigations\" : \"Replace {{Dummy1}}\"},"
              "{ \"code\": \"EC_DB_1\" ,  \"message\": \"Out of memory.\", "
              "\"parameters\" : [], \"mitigations\" : \"Buy more RAM\"},]", res);
}

TEST(ErrorMessage, BuildSimpleErrorMessage){
    const std::string res = EC_DB_1.str();
    EXPECT_EQ("EC_DB_1: Out of memory. Buy more RAM", res);
}

TEST(ErrorMessage, BuildParametrizedErrorMessage){
    const std::string res = EC_DB_2.build().setDummy1("/dev/sda1").setDummy2(100).str();
    EXPECT_EQ("EC_DB_2: hd error /dev/sda1 , 100. Replace /dev/sda1", res);
}