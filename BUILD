load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "common",
    srcs = glob(["common/*.cpp"]),
    hdrs = glob(["common/*.h"])
)

cc_library(
    name = "io",
    srcs = glob(["io/*.cpp"]),
    hdrs = glob(["io/*.h"])
)

cc_binary(
    name = "client",
    srcs = ["client/client.cpp"],
    deps = ["common", "io"]
)

cc_binary(
    name = "server",
    srcs = ["server/server.cpp"],
    deps = ["common", "io"]
)
