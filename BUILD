cc_library(
    name = "pd",
    hdrs = [
        "include/graph.h",
        "include/grow_subsets.h",
        "include/linear_function.h",
        "include/pd.h",
        "include/problem.h",
        "include/prune.h",
        "include/subset.h",
    ],
    srcs = [
        "src/graph.cpp",
        "src/grow_subsets.cpp",
        "src/linear_function.cpp",
        "src/pd.cpp",
        "src/prune.cpp",
        "src/subset.cpp",
    ],
    strip_include_prefix="include",
)

cc_library(
    name="read_file",
    hdrs = ["include/read_file.h"],
    srcs = ["src/read_file.cpp"],
    strip_include_prefix="include",
    deps = [":pd"],
)

filegroup(
    name="tsplib_benchmarks",
    srcs = glob(["tsplib_benchmarks/*"]),
)

cc_binary(
    name = "demo",
    srcs = ["src/demo.cpp"],
    data = [":tsplib_benchmarks"],
    deps = [
        ":pd",
        ":read_file",
    ],
)

cc_binary(
    name = "compile_baselines",
    srcs = ["src/compile_baselines.cpp"],
    data = [":tsplib_benchmarks"],
    deps = [
        ":pd",
        ":read_file",
    ],
)

cc_test(
    name = "linear_functions_test",
    srcs = ["test/linear_functions_test.cpp"],
    deps = [
        ":pd",
        "@googletest//:gtest_main"
    ],
)

cc_test(
    name = "read_files_test",
    srcs = ["test/read_file_test.cpp"],
    data = [":tsplib_benchmarks"],
    deps = [
        ":read_file",
        "@googletest//:gtest_main"
    ],
)

cc_test(
    name = "solution_baselines_test",
    srcs = [
        "test/solution_baselines.cpp",
        "test/baseline_database.h",
    ],
    data = [":tsplib_benchmarks"],
    deps = [
        ":pd",
        ":read_file",
        "@googletest//:gtest_main"
    ],
    size = "large",
)
