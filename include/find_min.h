#pragma once

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>
#include "graph.h"
#include "linear_function.h"

// Finds the minimum time for a subset to go tight
std::pair<double, std::shared_ptr<Subset>> minSetTime(
    const std::unordered_map<std::shared_ptr<Subset>, LinearFunctionPair>&
        lin_s,
    const std::list<std::shared_ptr<Subset>>& subsets, double t);

// Finds the minimum time for an edge to go tight
std::pair<double, EdgeFunctions> minEdgeTime(
    const std::vector<EdgeFunctions>& edge_functions, double t,
    double upper_bound);

// Resolves ties between min_time (needs to be cleaned up)
void resolveTies(
    double lambda, const std::vector<EdgeFunctions>& edge_functions,
    std::unordered_map<std::shared_ptr<Subset>, LinearFunctionPair>& lin_s,
    const EdgeFunctions& min_e_functions, std::shared_ptr<Edge>& alt_e,
    LinearFunction& lin_val, LinearFunction& lin_val_p1,
    LinearFunction& lin_val_p2);
