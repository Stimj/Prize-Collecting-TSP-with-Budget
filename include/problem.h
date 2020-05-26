
#pragma once

#include <list>

#include "graph.h"

// Helper structures to organize problem specification and solution information.
struct Problem {
  Graph graph;
  double budget;
  std::string name;
  // Optional: If non-empty, the solution tour must contain one of these nodes
  std::vector<int> roots;
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
