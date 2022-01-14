load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "demo-exe",
    srcs = ["src/demo_exe.cc"],
    visibility = ["//visibility:public"],
    deps= ["//demo/demo-lib"]
)