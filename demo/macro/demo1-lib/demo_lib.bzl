def demo_lib(name, defines):
    native.cc_library(
        name = name,
        srcs = ["src/demo_lib.h", "src/demo_lib_errors.h", "src/demo_lib_errors.cc", "src/demo_lib.cc"],
        visibility = ["//visibility:public"],
        deps = ["//error-reporting-cpp:error-reporting-cpp-macro"],
        defines = defines,
        alwayslink = True,
    )
