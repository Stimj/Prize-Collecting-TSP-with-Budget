#pragma once

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>
#include "graph.h"
#include "linear_function.h"

// Find min event to occur next
// Stores minimum subset pointer in min_s and minimum edge pointer in min_e
// Also finds event at lambda*(1+eps)
void findMinEvent(
    double lambda, const std::list<std::shared_ptr<Subset>>& subsets,
    std::vector<EdgeFunctions>& edge_functions,
    std::unordered_map<std::shared_ptr<Subset>, LinearFunctionPair>& lin_s,
    std::shared_ptr<Subset>& min_s, std::shared_ptr<Edge>& min_e,
    std::shared_ptr<Edge>& alt_e, LinearFunction& lin_val,
    LinearFunction& lin_val_p1, LinearFunction& lin_val_p2);

