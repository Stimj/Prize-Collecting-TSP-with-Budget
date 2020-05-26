cc_library(
    name = "json",
    srcs = [
        "src/to_json.cpp",
    ],
    hdrs = [
        "include/to_json.h",
    ],
    strip_include_prefix = "include",
    deps = [
        ":pd",
        "@json//:lib",
    ],
)

cc_library(
    name = "pd",
    srcs = [
        "src/graph.cpp",
        "src/grow_subsets.cpp",
        "src/linear_function.cpp",
        "src/pd.cpp",
        "src/prune.cpp",
        "src/subset.cpp",
    ],
    hdrs = [
        "include/graph.h",
        "include/grow_subsets.h",
        "include/linear_function.h",
        "include/pd.h",
        "include/problem.h",
        "include/prune.h",
        "include/subset.h",
    ],
    strip_include_prefix = "include",
)

cc_library(
    name = "read_file",
    srcs = ["src/read_file.cpp"],
    hdrs = [
        "include/problem.h",
        "include/read_file.h",
    ],
    strip_include_prefix = "include",
    deps = [":pd"],
)

filegroup(
    name = "tsplib_benchmarks",
    srcs = glob(["tsplib_benchmarks/*"]),
)

cc_binary(
    name = "demo",
    srcs = ["src/demo.cpp"],
    data = [
        ":tsplib_benchmarks",
        "@OPLib//:instances",
    ],
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

cc_binary(
    name = "characterize_complexity",
    srcs = ["src/characterize_complexity.cpp"],
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
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "read_files_test",
    srcs = ["test/read_file_test.cpp"],
    data = [
        ":tsplib_benchmarks",
        "@OPLib//:instances",
    ],
    deps = [
        ":read_file",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "solution_baselines_test",
    size = "large",
    srcs = [
        "test/baseline_database.h",
        "test/solution_baselines.cpp",
    ],
    data = [":tsplib_benchmarks"],
    deps = [
        ":pd",
        ":read_file",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "json_test",
    srcs = [
        "test/json_test.cpp",
    ],
    deps = [
        ":json",
        ":pd",
        "@googletest//:gtest_main",
    ],
)
