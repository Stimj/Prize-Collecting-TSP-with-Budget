
#pragma once

#include <list>

#include "graph.h"

// Helper structures to organize problem specification and solution information.
struct Problem {
  Graph graph;
  double budget;
  // Maximum time to run solver before terminating
  double time_limit;
};

struct Solution {
  bool solved;
  std::list<std::shared_ptr<Edge>> path;
  double prize;
  double upper_bound;
};

struct SolverInfo {
  // Problem to solve
  Problem problem;

  // Information about availability of solution and quality
  Solution solution;

  // Information about the algorithm run
  double lambda;
  int recursions;
  double walltime;
};
