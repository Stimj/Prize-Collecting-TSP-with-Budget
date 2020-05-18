#pragma once

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "graph.h"
#include "linear_function.h"

// Grow Function
// Runs the PD subroutine with lambda_1 = lambda and returns the end subsets

class GrowSubsets {
 public:
  GrowSubsets(double tieeps = 0.001, double eps = 1.0e-15)
      : tieeps_(tieeps), eps_(eps) {}

  std::list<std::shared_ptr<Subset>> build(const Graph& G, double lambda);

 private:
  // Linear search to find the minimum time until a set goes tight
  std::pair<double, std::shared_ptr<Subset>> minSetTime() const;

  // Linear search to find the minimum time until an edge goes tight
  std::pair<double, EdgeFunctions> minEdgeTime() const;

  // Resolves ties between events. Modifies lin_val* containers and may mark
  // some sets tied
  void resolveTies(const EdgeFunctions& min_e_functions);

  // Updates functions attached to each edge.
  // NOTE: this is the bottleneck (~40% of runtime)
  void updateEdge(EdgeFunctions* edge);

  // Updates edges given that a subset goes neutral first.
  // NOTE: ~25% of runtime on small problems is spent in the body of this
  // function.
  std::pair<double, EdgeFunctions> updateEdgesGivenNeutralSubset(
      const std::shared_ptr<Subset>& min_s);

  // Updates edges given that an edge goes tight first.
  // NOTE: ~25% of runtime for small problems, 40% for large problems is spent
  // in the body of this function.
  std::pair<double, EdgeFunctions> updateEdgesGivenTightEdge(
      const std::shared_ptr<Subset>& S1, const std::shared_ptr<Subset>& S2,
      const std::shared_ptr<Subset>& S);

  void updateSubsets();

  // Problem variables
  double tieeps_;
  double eps_;
  double t_minus_;
  double t_plus_;

  // Optimization variables
  std::unordered_map<int, std::shared_ptr<Subset>> vertex_subs_;
  std::unordered_map<std::shared_ptr<Subset>, LinearFunctionPair> lin_s_;
  std::vector<EdgeFunctions> edge_functions_;
  std::shared_ptr<Edge> alt_e_;
  // TODO: clean these lin_val_ names up
  LinearFunction lin_val_;
  LinearFunction lin_val_p1_;
  LinearFunction lin_val_p2_;
  LinearFunction lin_val_p1_plus_p2_;

  // Optimization outputs
  std::list<std::shared_ptr<Subset>> subsets_;
};
