#include <gtest/gtest.h>

#include <read_file.h>

size_t nodesInGraph(const std::string& filename, bool expect_success = true) {
  Problem problem;
  EXPECT_EQ(loadProblem(filename, problem), expect_success);
  return problem.graph.getVertices().size();
}

TEST(ReadFile, invalid_file) {
  EXPECT_EQ(0, nodesInGraph("does_not_exist", false));
}

TEST(ReadFile, DISABLED_edge_weight_tsp_files) {
  // These files use edge weights formats that are not parsed by read_files
  EXPECT_EQ(29, nodesInGraph("tsplib_benchmarks/bayg29.tsp"));
  EXPECT_EQ(29, nodesInGraph("tsplib_benchmarks/bays29.tsp"));
  EXPECT_EQ(158, nodesInGraph("tsplib_benchmarks/brazil158.tsp"));
  EXPECT_EQ(180, nodesInGraph("tsplib_benchmarks/brg180.tsp"));
  EXPECT_EQ(42, nodesInGraph("tsplib_benchmarks/dantzig42.tsp"));
  EXPECT_EQ(26, nodesInGraph("tsplib_benchmarks/fri26.tsp"));
  EXPECT_EQ(120, nodesInGraph("tsplib_benchmarks/gr120.tsp"));
  EXPECT_EQ(17, nodesInGraph("tsplib_benchmarks/gr17.tsp"));
  EXPECT_EQ(21, nodesInGraph("tsplib_benchmarks/gr21.tsp"));
  EXPECT_EQ(24, nodesInGraph("tsplib_benchmarks/gr24.tsp"));
  EXPECT_EQ(48, nodesInGraph("tsplib_benchmarks/gr48.tsp"));
  EXPECT_EQ(48, nodesInGraph("tsplib_benchmarks/hk48.tsp"));
  EXPECT_EQ(561, nodesInGraph("tsplib_benchmarks/pa561.tsp"));
  EXPECT_EQ(42, nodesInGraph("tsplib_benchmarks/swiss42.tsp"));
}

TEST(ReadFile, DISABLED_large_tsp_instances) {
  // These files are too large to be parsed reliably
  EXPECT_EQ(14051, nodesInGraph("tsplib_benchmarks/brd14051.tsp"));
  EXPECT_EQ(15112, nodesInGraph("tsplib_benchmarks/d15112.tsp"));
  EXPECT_EQ(18512, nodesInGraph("tsplib_benchmarks/d18512.tsp"));
  EXPECT_EQ(33810, nodesInGraph("tsplib_benchmarks/pla33810.tsp"));
  EXPECT_EQ(85900, nodesInGraph("tsplib_benchmarks/pla85900.tsp"));
  EXPECT_EQ(11849, nodesInGraph("tsplib_benchmarks/rl11849.tsp"));
  EXPECT_EQ(13509, nodesInGraph("tsplib_benchmarks/usa13509.tsp"));
}

TEST(ReadFile, DISABLED_node_coord_tsp_files) {
  EXPECT_EQ(280, nodesInGraph("tsplib_benchmarks/a280.tsp"));
  EXPECT_EQ(535, nodesInGraph("tsplib_benchmarks/ali535.tsp"));
  EXPECT_EQ(48, nodesInGraph("tsplib_benchmarks/att48.tsp"));
  EXPECT_EQ(532, nodesInGraph("tsplib_benchmarks/att532.tsp"));
  EXPECT_EQ(52, nodesInGraph("tsplib_benchmarks/berlin52.tsp"));
  EXPECT_EQ(127, nodesInGraph("tsplib_benchmarks/bier127.tsp"));
  EXPECT_EQ(14, nodesInGraph("tsplib_benchmarks/burma14.tsp"));
  EXPECT_EQ(130, nodesInGraph("tsplib_benchmarks/ch130.tsp"));
  EXPECT_EQ(150, nodesInGraph("tsplib_benchmarks/ch150.tsp"));
  EXPECT_EQ(1291, nodesInGraph("tsplib_benchmarks/d1291.tsp"));
  EXPECT_EQ(1655, nodesInGraph("tsplib_benchmarks/d1655.tsp"));
  EXPECT_EQ(198, nodesInGraph("tsplib_benchmarks/d198.tsp"));
  EXPECT_EQ(2103, nodesInGraph("tsplib_benchmarks/d2103.tsp"));
  EXPECT_EQ(493, nodesInGraph("tsplib_benchmarks/d493.tsp"));
  EXPECT_EQ(657, nodesInGraph("tsplib_benchmarks/d657.tsp"));
  EXPECT_EQ(1000, nodesInGraph("tsplib_benchmarks/dsj1000.tsp"));
  EXPECT_EQ(101, nodesInGraph("tsplib_benchmarks/eil101.tsp"));
  EXPECT_EQ(51, nodesInGraph("tsplib_benchmarks/eil51.tsp"));
  EXPECT_EQ(76, nodesInGraph("tsplib_benchmarks/eil76.tsp"));
  EXPECT_EQ(1400, nodesInGraph("tsplib_benchmarks/fl1400.tsp"));
  EXPECT_EQ(1577, nodesInGraph("tsplib_benchmarks/fl1577.tsp"));
  EXPECT_EQ(3795, nodesInGraph("tsplib_benchmarks/fl3795.tsp"));
  EXPECT_EQ(417, nodesInGraph("tsplib_benchmarks/fl417.tsp"));
  EXPECT_EQ(4461, nodesInGraph("tsplib_benchmarks/fnl4461.tsp"));
  EXPECT_EQ(262, nodesInGraph("tsplib_benchmarks/gil262.tsp"));
  EXPECT_EQ(137, nodesInGraph("tsplib_benchmarks/gr137.tsp"));
  EXPECT_EQ(202, nodesInGraph("tsplib_benchmarks/gr202.tsp"));
  EXPECT_EQ(229, nodesInGraph("tsplib_benchmarks/gr229.tsp"));
  EXPECT_EQ(431, nodesInGraph("tsplib_benchmarks/gr431.tsp"));
  EXPECT_EQ(666, nodesInGraph("tsplib_benchmarks/gr666.tsp"));
  EXPECT_EQ(96, nodesInGraph("tsplib_benchmarks/gr96.tsp"));
  EXPECT_EQ(100, nodesInGraph("tsplib_benchmarks/kroA100.tsp"));
  EXPECT_EQ(150, nodesInGraph("tsplib_benchmarks/kroA150.tsp"));
  EXPECT_EQ(200, nodesInGraph("tsplib_benchmarks/kroA200.tsp"));
  EXPECT_EQ(100, nodesInGraph("tsplib_benchmarks/kroB100.tsp"));
  EXPECT_EQ(150, nodesInGraph("tsplib_benchmarks/kroB150.tsp"));
  EXPECT_EQ(200, nodesInGraph("tsplib_benchmarks/kroB200.tsp"));
  EXPECT_EQ(100, nodesInGraph("tsplib_benchmarks/kroC100.tsp"));
  EXPECT_EQ(100, nodesInGraph("tsplib_benchmarks/kroD100.tsp"));
  EXPECT_EQ(100, nodesInGraph("tsplib_benchmarks/kroE100.tsp"));
  EXPECT_EQ(105, nodesInGraph("tsplib_benchmarks/lin105.tsp"));
  EXPECT_EQ(318, nodesInGraph("tsplib_benchmarks/lin318.tsp"));
  EXPECT_EQ(318, nodesInGraph("tsplib_benchmarks/linhp318.tsp"));
  EXPECT_EQ(1379, nodesInGraph("tsplib_benchmarks/nrw1379.tsp"));
  EXPECT_EQ(654, nodesInGraph("tsplib_benchmarks/p654.tsp"));
  EXPECT_EQ(1173, nodesInGraph("tsplib_benchmarks/pcb1173.tsp"));
  EXPECT_EQ(3038, nodesInGraph("tsplib_benchmarks/pcb3038.tsp"));
  EXPECT_EQ(442, nodesInGraph("tsplib_benchmarks/pcb442.tsp"));
  EXPECT_EQ(7397, nodesInGraph("tsplib_benchmarks/pla7397.tsp"));
  EXPECT_EQ(1002, nodesInGraph("tsplib_benchmarks/pr1002.tsp"));
  EXPECT_EQ(107, nodesInGraph("tsplib_benchmarks/pr107.tsp"));
  EXPECT_EQ(124, nodesInGraph("tsplib_benchmarks/pr124.tsp"));
  EXPECT_EQ(136, nodesInGraph("tsplib_benchmarks/pr136.tsp"));
  EXPECT_EQ(144, nodesInGraph("tsplib_benchmarks/pr144.tsp"));
  EXPECT_EQ(152, nodesInGraph("tsplib_benchmarks/pr152.tsp"));
  EXPECT_EQ(226, nodesInGraph("tsplib_benchmarks/pr226.tsp"));
  EXPECT_EQ(2392, nodesInGraph("tsplib_benchmarks/pr2392.tsp"));
  EXPECT_EQ(264, nodesInGraph("tsplib_benchmarks/pr264.tsp"));
  EXPECT_EQ(299, nodesInGraph("tsplib_benchmarks/pr299.tsp"));
  EXPECT_EQ(439, nodesInGraph("tsplib_benchmarks/pr439.tsp"));
  EXPECT_EQ(76, nodesInGraph("tsplib_benchmarks/pr76.tsp"));
  EXPECT_EQ(195, nodesInGraph("tsplib_benchmarks/rat195.tsp"));
  EXPECT_EQ(575, nodesInGraph("tsplib_benchmarks/rat575.tsp"));
  EXPECT_EQ(783, nodesInGraph("tsplib_benchmarks/rat783.tsp"));
  EXPECT_EQ(99, nodesInGraph("tsplib_benchmarks/rat99.tsp"));
  EXPECT_EQ(100, nodesInGraph("tsplib_benchmarks/rd100.tsp"));
  EXPECT_EQ(400, nodesInGraph("tsplib_benchmarks/rd400.tsp"));
  EXPECT_EQ(1304, nodesInGraph("tsplib_benchmarks/rl1304.tsp"));
  EXPECT_EQ(1323, nodesInGraph("tsplib_benchmarks/rl1323.tsp"));
  EXPECT_EQ(1889, nodesInGraph("tsplib_benchmarks/rl1889.tsp"));
  EXPECT_EQ(5915, nodesInGraph("tsplib_benchmarks/rl5915.tsp"));
  EXPECT_EQ(5934, nodesInGraph("tsplib_benchmarks/rl5934.tsp"));
  EXPECT_EQ(70, nodesInGraph("tsplib_benchmarks/st70.tsp"));
  EXPECT_EQ(225, nodesInGraph("tsplib_benchmarks/ts225.tsp"));
  EXPECT_EQ(225, nodesInGraph("tsplib_benchmarks/tsp225.tsp"));
  EXPECT_EQ(1060, nodesInGraph("tsplib_benchmarks/u1060.tsp"));
  EXPECT_EQ(1432, nodesInGraph("tsplib_benchmarks/u1432.tsp"));
  EXPECT_EQ(159, nodesInGraph("tsplib_benchmarks/u159.tsp"));
  EXPECT_EQ(1817, nodesInGraph("tsplib_benchmarks/u1817.tsp"));
  EXPECT_EQ(2152, nodesInGraph("tsplib_benchmarks/u2152.tsp"));
  EXPECT_EQ(2319, nodesInGraph("tsplib_benchmarks/u2319.tsp"));
  EXPECT_EQ(574, nodesInGraph("tsplib_benchmarks/u574.tsp"));
  EXPECT_EQ(724, nodesInGraph("tsplib_benchmarks/u724.tsp"));
  EXPECT_EQ(16, nodesInGraph("tsplib_benchmarks/ulysses16.tsp"));
  EXPECT_EQ(22, nodesInGraph("tsplib_benchmarks/ulysses22.tsp"));
  EXPECT_EQ(1084, nodesInGraph("tsplib_benchmarks/vm1084.tsp"));
  EXPECT_EQ(1748, nodesInGraph("tsplib_benchmarks/vm1748.tsp"));
}

TEST(ReadFile, oplib_gen1_files) {
  EXPECT_EQ(280,
            nodesInGraph("external/OPLib/instances/gen1/a280-gen1-50.oplib"));
  EXPECT_EQ(535,
            nodesInGraph("external/OPLib/instances/gen1/ali535-gen1-50.oplib"));
  EXPECT_EQ(48,
            nodesInGraph("external/OPLib/instances/gen1/att48-gen1-50.oplib"));
  EXPECT_EQ(532,
            nodesInGraph("external/OPLib/instances/gen1/att532-gen1-50.oplib"));
  EXPECT_EQ(
      52, nodesInGraph("external/OPLib/instances/gen1/berlin52-gen1-50.oplib"));
  EXPECT_EQ(
      52, nodesInGraph("external/OPLib/instances/gen1/berlin52-gen1-50.oplib"));
  EXPECT_EQ(
      127, nodesInGraph("external/OPLib/instances/gen1/bier127-gen1-50.oplib"));
  EXPECT_EQ(1291,
            nodesInGraph("external/OPLib/instances/gen1/d1291-gen1-50.oplib"));
  EXPECT_EQ(1655,
            nodesInGraph("external/OPLib/instances/gen1/d1655-gen1-50.oplib"));
  EXPECT_EQ(198,
            nodesInGraph("external/OPLib/instances/gen1/d198-gen1-50.oplib"));
  EXPECT_EQ(2103,
            nodesInGraph("external/OPLib/instances/gen1/d2103-gen1-50.oplib"));
  EXPECT_EQ(493,
            nodesInGraph("external/OPLib/instances/gen1/d493-gen1-50.oplib"));
  EXPECT_EQ(657,
            nodesInGraph("external/OPLib/instances/gen1/d657-gen1-50.oplib"));
  EXPECT_EQ(1000, nodesInGraph(
                      "external/OPLib/instances/gen1/dsj1000-gen1-50.oplib"));
  EXPECT_EQ(101,
            nodesInGraph("external/OPLib/instances/gen1/eil101-gen1-50.oplib"));
  EXPECT_EQ(51,
            nodesInGraph("external/OPLib/instances/gen1/eil51-gen1-50.oplib"));
  EXPECT_EQ(76,
            nodesInGraph("external/OPLib/instances/gen1/eil76-gen1-50.oplib"));
  EXPECT_EQ(1400,
            nodesInGraph("external/OPLib/instances/gen1/fl1400-gen1-50.oplib"));
  EXPECT_EQ(1577,
            nodesInGraph("external/OPLib/instances/gen1/fl1577-gen1-50.oplib"));
  EXPECT_EQ(3795,
            nodesInGraph("external/OPLib/instances/gen1/fl3795-gen1-50.oplib"));
  EXPECT_EQ(417,
            nodesInGraph("external/OPLib/instances/gen1/fl417-gen1-50.oplib"));
  EXPECT_EQ(4461, nodesInGraph(
                      "external/OPLib/instances/gen1/fnl4461-gen1-50.oplib"));
  EXPECT_EQ(262,
            nodesInGraph("external/OPLib/instances/gen1/gil262-gen1-50.oplib"));
  EXPECT_EQ(137,
            nodesInGraph("external/OPLib/instances/gen1/gr137-gen1-50.oplib"));
  EXPECT_EQ(202,
            nodesInGraph("external/OPLib/instances/gen1/gr202-gen1-50.oplib"));
  EXPECT_EQ(229,
            nodesInGraph("external/OPLib/instances/gen1/gr229-gen1-50.oplib"));
  EXPECT_EQ(431,
            nodesInGraph("external/OPLib/instances/gen1/gr431-gen1-50.oplib"));
  EXPECT_EQ(666,
            nodesInGraph("external/OPLib/instances/gen1/gr666-gen1-50.oplib"));
  EXPECT_EQ(96,
            nodesInGraph("external/OPLib/instances/gen1/gr96-gen1-50.oplib"));
  EXPECT_EQ(
      100, nodesInGraph("external/OPLib/instances/gen1/kroA100-gen1-50.oplib"));
  EXPECT_EQ(
      150, nodesInGraph("external/OPLib/instances/gen1/kroA150-gen1-50.oplib"));
  EXPECT_EQ(
      200, nodesInGraph("external/OPLib/instances/gen1/kroA200-gen1-50.oplib"));
  EXPECT_EQ(
      100, nodesInGraph("external/OPLib/instances/gen1/kroB100-gen1-50.oplib"));
  EXPECT_EQ(
      150, nodesInGraph("external/OPLib/instances/gen1/kroB150-gen1-50.oplib"));
  EXPECT_EQ(
      200, nodesInGraph("external/OPLib/instances/gen1/kroB200-gen1-50.oplib"));
  EXPECT_EQ(
      100, nodesInGraph("external/OPLib/instances/gen1/kroC100-gen1-50.oplib"));
  EXPECT_EQ(
      100, nodesInGraph("external/OPLib/instances/gen1/kroD100-gen1-50.oplib"));
  EXPECT_EQ(
      100, nodesInGraph("external/OPLib/instances/gen1/kroE100-gen1-50.oplib"));
  EXPECT_EQ(105,
            nodesInGraph("external/OPLib/instances/gen1/lin105-gen1-50.oplib"));
  EXPECT_EQ(318,
            nodesInGraph("external/OPLib/instances/gen1/lin318-gen1-50.oplib"));
  EXPECT_EQ(1379, nodesInGraph(
                      "external/OPLib/instances/gen1/nrw1379-gen1-50.oplib"));
  EXPECT_EQ(654,
            nodesInGraph("external/OPLib/instances/gen1/p654-gen1-50.oplib"));
  EXPECT_EQ(1173, nodesInGraph(
                      "external/OPLib/instances/gen1/pcb1173-gen1-50.oplib"));
  EXPECT_EQ(3038, nodesInGraph(
                      "external/OPLib/instances/gen1/pcb3038-gen1-50.oplib"));
  EXPECT_EQ(442,
            nodesInGraph("external/OPLib/instances/gen1/pcb442-gen1-50.oplib"));
  EXPECT_EQ(7397, nodesInGraph(
                      "external/OPLib/instances/gen1/pla7397-gen1-50.oplib"));
  EXPECT_EQ(1002,
            nodesInGraph("external/OPLib/instances/gen1/pr1002-gen1-50.oplib"));
  EXPECT_EQ(107,
            nodesInGraph("external/OPLib/instances/gen1/pr107-gen1-50.oplib"));
  EXPECT_EQ(124,
            nodesInGraph("external/OPLib/instances/gen1/pr124-gen1-50.oplib"));
  EXPECT_EQ(136,
            nodesInGraph("external/OPLib/instances/gen1/pr136-gen1-50.oplib"));
  EXPECT_EQ(144,
            nodesInGraph("external/OPLib/instances/gen1/pr144-gen1-50.oplib"));
  EXPECT_EQ(152,
            nodesInGraph("external/OPLib/instances/gen1/pr152-gen1-50.oplib"));
  EXPECT_EQ(226,
            nodesInGraph("external/OPLib/instances/gen1/pr226-gen1-50.oplib"));
  EXPECT_EQ(2392,
            nodesInGraph("external/OPLib/instances/gen1/pr2392-gen1-50.oplib"));
  EXPECT_EQ(264,
            nodesInGraph("external/OPLib/instances/gen1/pr264-gen1-50.oplib"));
  EXPECT_EQ(299,
            nodesInGraph("external/OPLib/instances/gen1/pr299-gen1-50.oplib"));
  EXPECT_EQ(439,
            nodesInGraph("external/OPLib/instances/gen1/pr439-gen1-50.oplib"));
  EXPECT_EQ(76,
            nodesInGraph("external/OPLib/instances/gen1/pr76-gen1-50.oplib"));
  EXPECT_EQ(195,
            nodesInGraph("external/OPLib/instances/gen1/rat195-gen1-50.oplib"));
  EXPECT_EQ(575,
            nodesInGraph("external/OPLib/instances/gen1/rat575-gen1-50.oplib"));
  EXPECT_EQ(783,
            nodesInGraph("external/OPLib/instances/gen1/rat783-gen1-50.oplib"));
  EXPECT_EQ(99,
            nodesInGraph("external/OPLib/instances/gen1/rat99-gen1-50.oplib"));
  EXPECT_EQ(100,
            nodesInGraph("external/OPLib/instances/gen1/rd100-gen1-50.oplib"));
  EXPECT_EQ(400,
            nodesInGraph("external/OPLib/instances/gen1/rd400-gen1-50.oplib"));
  EXPECT_EQ(1304,
            nodesInGraph("external/OPLib/instances/gen1/rl1304-gen1-50.oplib"));
  EXPECT_EQ(1323,
            nodesInGraph("external/OPLib/instances/gen1/rl1323-gen1-50.oplib"));
  EXPECT_EQ(1889,
            nodesInGraph("external/OPLib/instances/gen1/rl1889-gen1-50.oplib"));
  EXPECT_EQ(5915,
            nodesInGraph("external/OPLib/instances/gen1/rl5915-gen1-50.oplib"));
  EXPECT_EQ(5934,
            nodesInGraph("external/OPLib/instances/gen1/rl5934-gen1-50.oplib"));
  EXPECT_EQ(70,
            nodesInGraph("external/OPLib/instances/gen1/st70-gen1-50.oplib"));
  EXPECT_EQ(225,
            nodesInGraph("external/OPLib/instances/gen1/ts225-gen1-50.oplib"));
  EXPECT_EQ(225,
            nodesInGraph("external/OPLib/instances/gen1/tsp225-gen1-50.oplib"));
  EXPECT_EQ(1060,
            nodesInGraph("external/OPLib/instances/gen1/u1060-gen1-50.oplib"));
  EXPECT_EQ(1432,
            nodesInGraph("external/OPLib/instances/gen1/u1432-gen1-50.oplib"));
  EXPECT_EQ(159,
            nodesInGraph("external/OPLib/instances/gen1/u159-gen1-50.oplib"));
  EXPECT_EQ(1817,
            nodesInGraph("external/OPLib/instances/gen1/u1817-gen1-50.oplib"));
  EXPECT_EQ(2152,
            nodesInGraph("external/OPLib/instances/gen1/u2152-gen1-50.oplib"));
  EXPECT_EQ(2319,
            nodesInGraph("external/OPLib/instances/gen1/u2319-gen1-50.oplib"));
  EXPECT_EQ(574,
            nodesInGraph("external/OPLib/instances/gen1/u574-gen1-50.oplib"));
  EXPECT_EQ(724,
            nodesInGraph("external/OPLib/instances/gen1/u724-gen1-50.oplib"));
  EXPECT_EQ(1084,
            nodesInGraph("external/OPLib/instances/gen1/vm1084-gen1-50.oplib"));
  EXPECT_EQ(1748,
            nodesInGraph("external/OPLib/instances/gen1/vm1748-gen1-50.oplib"));
}
