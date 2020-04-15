#pragma once

// Defines a linear function y = ax + b
struct LinearFunction {
  double a;
  double b;
};

// Used to store a pair of linear functions
struct LinearFunctionPair {
  LinearFunction first;
  LinearFunction second;
};
