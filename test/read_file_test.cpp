#include "gtest/gtest.h"

#include "read_file.h"

int nodesInGraph(const std::string& filename) {
  // Read graph file and return number of nodes
  Graph G;
  double meanEdgeWeight;
  int numNodes;
  std::string name;
  graphFromFile(filename, G, name, meanEdgeWeight, numNodes);
  return numNodes;
}

TEST(ReadFile, invalid_file) {
  EXPECT_EQ(0, nodesInGraph("does_not_exist"));
}
TEST(ReadFile, edge_weight_tsp_files) {
  // These files use edge weights formats that are not parsed by read_files
  /*
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
  EXPECT_EQ(42, nodesInGraph("tsplib_benchmarks/swiss42.tsp"));
  */
}

TEST(ReadFile, large_tsp_instances) {
  // These files are too large to be parsed reliably
  /*
  EXPECT_EQ(14051, nodesInGraph("tsplib_benchmarks/brd14051.tsp"));
  EXPECT_EQ(15112, nodesInGraph("tsplib_benchmarks/d15112.tsp"));
  EXPECT_EQ(18512, nodesInGraph("tsplib_benchmarks/d18512.tsp"));
  EXPECT_EQ(33810, nodesInGraph("tsplib_benchmarks/pla33810.tsp"));
  EXPECT_EQ(85900, nodesInGraph("tsplib_benchmarks/pla85900.tsp"));
  EXPECT_EQ(11849, nodesInGraph("tsplib_benchmarks/rl11849.tsp"));
  EXPECT_EQ(13509, nodesInGraph("tsplib_benchmarks/usa13509.tsp"));
 */

}

TEST(ReadFile, node_coord_tsp_files) {
  EXPECT_EQ(280, nodesInGraph("tsplib_benchmarks/a280.tsp"));
  EXPECT_EQ(535, nodesInGraph("tsplib_benchmarks/ali535.tsp"));
  EXPECT_EQ(48, nodesInGraph("tsplib_benchmarks/att48.tsp"));
  EXPECT_EQ(532, nodesInGraph("tsplib_benchmarks/att532.tsp"));
  EXPECT_EQ(52, nodesInGraph("tsplib_benchmarks/berlin52.tsp"));
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
// TODO: Fix parser error
//  EXPECT_EQ(318, nodesInGraph("tsplib_benchmarks/linhp318.tsp"));
  EXPECT_EQ(1379, nodesInGraph("tsplib_benchmarks/nrw1379.tsp"));
  EXPECT_EQ(654, nodesInGraph("tsplib_benchmarks/p654.tsp"));
// TODO: Fix parser error
//  EXPECT_EQ(561, nodesInGraph("tsplib_benchmarks/pa561.tsp"));
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

