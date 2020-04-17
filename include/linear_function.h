#pragma once
#include <memory>
#include "graph.h"
#include "subset.h"

// Defines a linear function y = ax + b
struct LinearFunction {
  double a;
  double b;

  inline double operator()(double t) const {
    return a * t + b;
  }
};

// Used to store a pair of linear functions
struct LinearFunctionPair {
  LinearFunction first;
  LinearFunction second;
};

struct EdgeFunctions {
  std::shared_ptr<Edge> edge;
  LinearFunction first;
  LinearFunction second;
  std::shared_ptr<Subset> p1;
  std::shared_ptr<Subset> p2;
};

