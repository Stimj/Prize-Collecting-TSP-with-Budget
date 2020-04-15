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

int main(int argc, char* argv[]) {
  std::string file = "bier127.tsp";

  SolverInfo info;

  double mean_edge_weight;
  int num_nodes;
  // read the graph from the file and get some stats from it
  auto success = graphFromFile("tsplib_benchmarks/" + file, info.problem.graph,
                               mean_edge_weight, num_nodes);
  if(!success) {
    std::cout << "Error reading tsp file\n";
    return 1;
  }

  std::list<std::shared_ptr<Edge>> mst;
  info.problem.budget = 0.5 * info.problem.graph.MST(mst);

  solveInstance(info);

  std::cout << "Search finished after " << info.walltime << " seconds\n";
  if (info.solution.solved) {
    std::cout << "\nUpper bound: " << info.solution.upper_bound
              << "\nPrize: " << info.solution.prize << "\nOptimality gap: "
              << (info.solution.upper_bound - info.solution.prize) /
                     info.solution.upper_bound
              << std::endl;
  } else {
    std::cout << "Did not find a solution\n";
  }
  std::cout << std::endl;
  return 0;
}
