#include "gtest/gtest.h"

#include "graph.h"
#include "pd.h"
#include "read_file.h"
#include "subset.h"

// Defines global kBaselineDatabase
#include "baseline_database.h"

TEST(SolutionBaselines, baseline_database) {
  for (const auto& kv : kBaselineDatabase) {
    SolverInfo info;
    ASSERT_TRUE(loadProblem("tsplib_benchmarks/" + kv.first, info.problem));
    info.problem.budget = kv.second.problem.budget;
    info.problem.time_limit = 300;

    // Now solve the problem
    solveInstance(info);
    ASSERT_TRUE(info.solution.solved);

    // Upper bound should not increase
    ASSERT_LE(info.solution.upper_bound - kv.second.solution.upper_bound,
              0.001);
    // Prize should not decrease
    ASSERT_GE(info.solution.prize - kv.second.solution.prize, -0.001);
  }
}
