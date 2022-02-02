load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test-error-reporting-cpp-macro",
    srcs = ["test.cpp", "test_main.cpp", "test_error_messages.h"],
    deps = [
        "//error-reporting-cpp:error-reporting-cpp-macro",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "test-error-reporting-cpp-with-collection-macro",
    srcs = ["test.cpp", "test_main.cpp", "test_error_messages.h"],
    deps = [
        "//error-reporting-cpp:error-reporting-cpp-macro",
        "@googletest//:gtest_main",
    ],
    defines = ["ERROR_MESSAGE_COLLECTION"],
)
