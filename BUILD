cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [":crawler"]
)

cc_library(
    name = "crawler",
    srcs = ["crawler.cpp"],
    hdrs = ["crawler.h"],
    deps = ["//thread_safe:thread_safe_map",
            "//thread_safe:thread_safe_queue",
            "//thread_safe:thread_safe_var"],
    linkopts = ["-lcurl"],
)



