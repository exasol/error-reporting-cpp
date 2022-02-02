load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "demo-crawler",
    srcs = ["src/demo_crawler.cc"],
    visibility = ["//visibility:public"],
    deps= [
        "//demo/macro/demo1-lib:demo1-lib-with-collection",
        "//demo/macro/demo2-lib:demo2-lib-with-collection"
    ],
    defines= ["ERROR_MESSAGE_COLLECTION"],
)
