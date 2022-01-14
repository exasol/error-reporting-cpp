load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "demo-lib",
    srcs = ["src/demo_lib.h", "src/demo_lib_errors.h", "src/demo_lib_errors.cc", "src/demo_lib.cc"],
    visibility = ["//visibility:public"],
    deps=["//error-reporting-cpp:error-reporting-cpp"]
)