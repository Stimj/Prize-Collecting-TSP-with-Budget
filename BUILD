cc_library(
    name = "pd",
    hdrs = ["include/graph.h", "include/subset.h", "include/subroutine.h", "include/pd.h"],
    srcs = ["src/graph.cpp", "src/subset.cpp", "src/subroutine.cpp", "src/pd.cpp"],
    strip_include_prefix="include"
)

cc_library(
    name="read_file",
    hdrs = ["include/read_file.h"],
    srcs = ["src/read_file.cpp"],
    strip_include_prefix="include",
    deps = [":pd"]
)

filegroup(
    name="tsplib_benchmarks",
    srcs = glob(["tsplib_benchmarks/*"])
)

cc_binary(
    name = "demo",
    srcs = [
        "src/demo.cpp",
    ],
    data = [":tsplib_benchmarks"],
    deps = [":pd", ":read_file"],
)
