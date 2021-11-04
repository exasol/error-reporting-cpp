#include "include/gtest/gtest.h"
#include <string>

TEST(ErrorMessage, BasicErrorMessage){

    std::string actual = "Hello Wor;d";
    std::string expected = "Hello World!";
    EXPECT_EQ(expected, actual);
}