load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "demo-crawler",
    srcs = ["src/demo_crawler.cc"],
    visibility = ["//visibility:public"],
    deps= ["//demo/demo-lib:demo-lib-with-collection"],
    defines= ["ERROR_MESSAGE_COLLECTION"]
)