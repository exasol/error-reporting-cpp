load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "demo2-lib",
    srcs = ["src/demo_lib.h", "src/demo_lib_errors.h", "src/demo_lib_errors.cc", "src/demo_lib.cc"],
    visibility = ["//visibility:public"],
    deps=["//error-reporting-cpp:error-reporting-cpp"]
)

cc_library(
    name = "demo2-lib-with-collection",
    srcs = ["src/demo_lib.h", "src/demo_lib_errors.h", "src/demo_lib_errors.cc", "src/demo_lib.cc"],
    visibility = ["//visibility:public"],
    deps=["//error-reporting-cpp:error-reporting-cpp"],
    defines= ["ERROR_MESSAGE_COLLECTION"],
    alwayslink = True
)
