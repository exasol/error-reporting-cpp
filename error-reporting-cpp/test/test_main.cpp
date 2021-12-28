
#define MAIN_ERROR_REPORTING_CPP
#include "test_error_messages.h"
#include "include/gtest/gtest.h"

TEST(ErrorMessageInternal, CheckErrorCode) {
    const std::string ec = std::string(EC_DB_1.error_code_str);
    EXPECT_EQ("EC_DB_1", ec);
}

TEST(ErrorMessageInternal, CheckMitigation) {
    const std::string ec = std::string(EC_DB_1.mitigations_str);
    EXPECT_EQ("Buy more RAM", ec);
}

TEST(ErrorMessageInternal, CheckDescription) {
    const std::string ec = std::string(EC_DB_1.message_str);
    EXPECT_EQ("Out of memory.", ec);
}

TEST(ErrorMessageInternal, CheckErrorCodeDb2) {
    const std::string ec = std::string(EC_DB_2.error_code_str);
    EXPECT_EQ("EC_DB_2", ec);
}

TEST(ErrorMessageInternal, CheckMitigationDb2) {
    const std::string ec = std::string(EC_DB_2.mitigations_str);
    EXPECT_EQ( "Replace {{Dummy1}}", ec);
}

TEST(ErrorMessageInternal, CheckDescriptipnDb2) {
    const std::string ec = std::string(EC_DB_2.message_str);
    EXPECT_EQ("hd error {{Dummy1}} , {{Dummy2}}.", ec);
}



TEST(ErrorMessageInternal, CheckDescriptipnDb3) {
    const std::string ec = std::string(EC_DB_3.mitigations_str);
    EXPECT_EQ(ec.empty(), true);
}
