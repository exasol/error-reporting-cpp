load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test-error-reporting-cpp",
    srcs = ["test.cpp", "test_main.cpp", "test_error_messages.h"],
    deps = [
        "//error-reporting-cpp",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "test-error-reporting-cpp-with-collection",
    srcs = ["test.cpp", "test_main.cpp", "test_error_messages.h"],
    deps = [
        "//error-reporting-cpp",
        "@googletest//:gtest_main",
    ],
    defines = ["ERROR_MESSAGE_COLLECTION"],
)
