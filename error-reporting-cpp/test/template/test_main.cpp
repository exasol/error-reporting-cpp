#define CREATE_ERROR_MESSAGE_CLASS_INSTANCES
#define MAIN_ERROR_REPORTING_CPP
#include "error-reporting-cpp/test/template/test_error_messages.h"
#include "include/gtest/gtest.h"

TEST(ErrorMessageInternal, CheckErrorCode) {
    const std::string ec = std::string(EC_DB_1.code);
    EXPECT_EQ("EC_DB_1", ec);
}

TEST(ErrorMessageInternal, CheckMitigation) {
    const auto ec = EC_DB_1.mitigations;
    const std::vector<std::string> expected = {"Buy more RAM"};
    EXPECT_EQ(expected, ec);
}

TEST(ErrorMessageInternal, CheckDescription) {
    const std::string ec = std::string(EC_DB_1.message);
    EXPECT_EQ("Out of memory.", ec);
}

TEST(ErrorMessageInternal, CheckErrorCodeDb2) {
    const std::string ec = std::string(EC_DB_2.code);
    EXPECT_EQ("EC_DB_2", ec);
}

TEST(ErrorMessageInternal, CheckMitigationDb2) {
    const auto ec = EC_DB_2.mitigations;
    const std::vector<std::string> expected = { "Replace {{Dummy1}}" };
    EXPECT_EQ(expected, ec);
}

TEST(ErrorMessageInternal, CheckDescriptipnDb2) {
    const std::string ec = std::string(EC_DB_2.message);
    EXPECT_EQ("hd error {{Dummy1}} , {{Dummy2}}.", ec);
}

TEST(ErrorMessageInternal, CheckDescriptipnDb3) {
    const auto ec = EC_DB_3.mitigations;
    EXPECT_EQ(ec.empty(), true);
}
