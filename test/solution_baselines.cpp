#include "gtest/gtest.h"

#include "graph.h"
#include "pd.h"
#include "read_file.h"
#include "subset.h"

// Defines global kBaselineDatabase
#include "baseline_database.h"

TEST(SolutionBaselines, baseline_database) {
  double mean_edge_weight;
  int num_nodes;
  for (const auto& kv : kBaselineDatabase) {

    SolverInfo info;
    auto read_success =
        graphFromFile("tsplib_benchmarks/" + kv.first, info.problem.graph,
                      mean_edge_weight, num_nodes);
    /* Uncomment to benchmark on only problems up to 400 nodes (same benchmark run in original paper)
    if(num_nodes > 400)
      continue;
    */
    ASSERT_TRUE(read_success);
    info.problem.budget = kv.second.problem.budget;

    // Now solve the problem
    solveInstance(info);
    ASSERT_TRUE(info.solution.solved);

    // Upper bound should not increase
    ASSERT_LE(info.solution.upper_bound - kv.second.solution.upper_bound, 0.001);
    // Prize should not decrease
    ASSERT_GE(info.solution.prize - kv.second.solution.prize, -0.001);
  }
}
