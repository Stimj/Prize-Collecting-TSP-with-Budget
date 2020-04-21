//
//  demo.cpp
//  Prize Collecting TSP
//
//  Created by Alice Paul on 3/14/17.

//  MIT License
//  Copyright (c) 2020 alicepaul

#include <list>
#include <memory>
#include <string>

#include "graph.h"
#include "pd.h"
#include "read_file.h"

/**
 * @file Example which shows how to load and solve a problem from the TSPLIB
 * dataset.
 */

int main(int argc, char* argv[]) {
  // Configure solution parameters:
  SolverInfo info;
  info.problem.time_limit = 500;  // maximum seconds to let the solver run

  // Read graph from file and load to SolverInfo.problem.graph
  std::string file = "bier127.tsp";
  double mean_edge_weight;
  int num_nodes;
  if (!graphFromFile("tsplib_benchmarks/" + file, info.problem.graph,
                     mean_edge_weight, num_nodes)) {
    std::cout << "Error reading tsp file\n";
    return 1;
  }

  // Pick budget for the problem by solving minimum spanning tree problem.
  // This can be replaced with any positive budget.
  std::list<std::shared_ptr<Edge>> mst;
  info.problem.budget = 0.5 * info.problem.graph.MST(mst);

  // Solve the problem and print stats
  solveInstance(info);

  std::cout << "Search finished after " << info.walltime << " seconds\n";
  if (info.solution.solved) {
    std::cout << "\nUpper bound: " << info.solution.upper_bound
              << "\nPrize: " << info.solution.prize
              << "\nOptimality gap (lower is better): "
              << (info.solution.upper_bound - info.solution.prize) /
                     info.solution.upper_bound
              << std::endl;
  } else {
    std::cout << "Did not find a solution\n";
  }
  std::cout << std::endl;
  return 0;
}
