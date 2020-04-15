//
//  main.cpp
//  Prize Collecting TSP
//
//  Created by Alice Paul on 3/14/17.

//  MIT License
//  Copyright (c) 2020 alicepaul

#include <stdio.h>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "graph.h"
#include "pd.h"
#include "read_file.h"
#include "subroutine.h"
#include "subset.h"

int main(int argc, char* argv[]) {
  std::string file = "bier127.tsp";

  SolverInfo info;

  double mean_edge_weight;
  int num_nodes;
  // read the graph from the file and get some stats from it
  auto success = graphFromFile("tsplib_benchmarks/" + file, info.problem.graph,
                               mean_edge_weight, num_nodes);

  std::list<std::shared_ptr<Edge>> mst;
  info.problem.budget = 0.5 * info.problem.graph.MST(mst);

  solveInstance(info);

  std::cout << "Search finished after " << info.walltime << " seconds\n";
  std::cout << "Solution found? " << info.solution.solved
            << "\nUpper bound: " << info.solution.upper_bound
            << "\nPrize: " << info.solution.prize << std::endl;
}
