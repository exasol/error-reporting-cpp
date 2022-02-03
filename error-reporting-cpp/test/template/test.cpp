#include "error-reporting-cpp/src/template/error_message.h"
#include "error-reporting-cpp/test/template/test_error_messages.h"
#include "include/gtest/gtest.h"
#include <string>


using namespace error_reporting;




TEST(ErrorMessage, BasicErrorMessage){
    const std::string res = g_error_message_container.collect_error_messages_as_json();

    const std::string expected =
#ifdef ERROR_MESSAGE_COLLECTION
        "[{ \"code\": \"EC_DB_3\" ,  \"message\": \"Kernel panic\", "
        "\"parameters\" : [], \"mitigations\" : []}, "
        "{ \"code\": \"EC_DB_2\" ,  \"message\": \"hd error {{Dummy1}} , {{Dummy2}}.\", "
        "\"parameters\" : [\"Description for Dummy1\",\"Description for Dummy2\",], "
        "\"mitigations\" : [\"Replace {{Dummy1}}\",]}, "
        "{ \"code\": \"EC_DB_1\" ,  \"message\": \"Out of memory.\", "
        "\"parameters\" : [], \"mitigations\" : [\"Buy more RAM\",]}, ]";
#else
    "[]";
#endif
    EXPECT_EQ(expected, res);
}

TEST(ErrorMessage, BuildSimpleErrorMessage){
    const std::string res = EC_DB_1.build();
    EXPECT_EQ("EC_DB_1: Out of memory. Buy more RAM", res);
}


TEST(ErrorMessage, BuildSimpleErrorMessageNoMitigation){
    const std::string res = EC_DB_3.build();
    EXPECT_EQ("EC_DB_3: Kernel panic", res);
}

TEST(ErrorMessage, BuildParametrizedErrorMessage){
    const std::string res = EC_DB_2.build(std::string("/dev/sda1"), 100);
    EXPECT_EQ("EC_DB_2: hd error '/dev/sda1' , '100'. Replace '/dev/sda1'", res);
}
