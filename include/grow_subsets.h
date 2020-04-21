#pragma once

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "graph.h"
#include "linear_function.h"

// Grow Function
// Runs the PD subroutine with lambda_1 = lambda and returns the end subsets

std::list<std::shared_ptr<Subset>> growSubsets(const Graph& G, double lambda);
