#include "find_min.h"

// Linear search through subsets to find next subset which goes tight
std::pair<double, std::shared_ptr<Subset>> minSetTime(
    const std::unordered_map<std::shared_ptr<Subset>, LinearFunctionPair>&
        lin_s,
    const std::list<std::shared_ptr<Subset>>& subsets, double t) {
  double eps = 1.0e-15;  // Level of precision for general ties
  double time_s = INT_MAX;
  std::shared_ptr<Subset> min_s = nullptr;
  for (const auto& s : subsets) {
    if (!s->getActive()) continue;

    double tight_time = lin_s.at(s).first(t);
    if (tight_time < time_s - eps) {  // break ties by slope
      time_s = tight_time;
      min_s = s;
    }
  }

  return std::make_pair(time_s, min_s);
}

// Linear search through edges to find next edge which goes tight
// NOTE: This is a bottleneck, currently 50% of instructions are here.
std::pair<double, EdgeFunctions> minEdgeTime(
    const std::vector<EdgeFunctions>& edge_functions, double t,
    double upper_bound) {
  double eps = 1.0e-15;  // Level of precision for general ties
  double time_e = upper_bound;

  const EdgeFunctions* optimizer = nullptr;
  for (const auto& e : edge_functions) {
    if (!e.p1->getActive() && !e.p2->getActive()) continue;
    if (e.p1 == e.p2) continue;

    // Time to go tight for edge e
    double tight_time =
        e.first(t) / (int(e.p1->getActive()) + int(e.p2->getActive()));

    // If new min store constant reference (avoid copying until loop finishes)
    if (tight_time < time_e - eps) {
      time_e = tight_time;
      optimizer = &e;
    }
  }

  EdgeFunctions min_e_functions;
  if (optimizer != nullptr) {
    min_e_functions.edge = optimizer->edge;
    min_e_functions.first = optimizer->first;
    min_e_functions.second = optimizer->second;
    min_e_functions.p1 = optimizer->p1;
    min_e_functions.p2 = optimizer->p2;
  }
  return std::make_pair(time_e, min_e_functions);
}

// Find min event to occur next
void findMinEvent(
    double lambda, const std::list<std::shared_ptr<Subset>>& subsets,
    std::vector<EdgeFunctions>& edge_functions,
    std::unordered_map<std::shared_ptr<Subset>, LinearFunctionPair>& lin_s,
    std::shared_ptr<Subset>& min_s, std::shared_ptr<Edge>& min_e,
    std::shared_ptr<Edge>& alt_e, LinearFunction& lin_val,
    LinearFunction& lin_val_p1, LinearFunction& lin_val_p2) {
  double eps = 1.0e-15;   // Level of precision for general ties
  double tieeps = 0.001;  // Level of precisions to count ties

  // Find minimum event between active subsets and active edges
  auto min_set = minSetTime(lin_s, subsets, lambda * (1 - tieeps));
  auto time_s = min_set.first;
  min_s = min_set.second;

  auto min_edge =
      minEdgeTime(edge_functions, lambda * (1 - tieeps), time_s + eps);
  double time_e = min_edge.first;
  EdgeFunctions min_e_functions = min_edge.second;
  min_e = min_e_functions.edge;

  // This means that the search is done.
  if (min_s == nullptr && min_e == nullptr) {
    return;
  }

  if (time_e < time_s + eps) {
    min_s = nullptr;
    double factor = 1.0 / (int(min_e_functions.p1->getActive()) +
                           int(min_e_functions.p2->getActive()));
    lin_val = {factor * min_e_functions.first.a,
               factor * min_e_functions.first.b};
  } else {
    min_e = nullptr;
    lin_val = lin_s.at(min_s).first;
  }

  lin_val_p1 = {0., 0.}, lin_val_p2 = {lin_val.a, lin_val.b};
  alt_e = min_e;

  // If subset event, we're done
  if (min_e == nullptr) return;

  // If an edge event then we have to check for ties
  // Find event time for min_e at lambda*(1+tieeps)
  double factor = 1.0 / (int(min_e_functions.p1->getActive()) +
                         int(min_e_functions.p2->getActive()));
  double time_p = factor * min_e_functions.second(lambda * (1 + tieeps));

  // Find minimum tied edge between same subsets at lambda*(1+tieeps)
  const EdgeFunctions* optimizer = nullptr;
  for (const auto& e : edge_functions) {
    if (((e.p1 == min_e_functions.p1) && (e.p2 == min_e_functions.p2)) ||
        ((e.p1 == min_e_functions.p2) && (e.p2 == min_e_functions.p1))) {
      // Time to go tight for edge e
      double tight_time = factor * e.second(lambda * (1 + tieeps));

      // If new min at lambda*(1+tieeps)
      if (tight_time < time_p - eps) {
        optimizer = &e;
        time_p = tight_time;
      }
    }
  }
  if (optimizer != nullptr) {
    lin_val_p2 = {factor * optimizer->second.a, factor * optimizer->second.b};
    alt_e = optimizer->edge;
  }

  // Find if parents go neutral before the edge at lambda*(1+eps)
  double testp1 = INT_MAX, testp2 = INT_MAX;
  bool tiedp1 = false, tiedp2 = false;
  if (min_e_functions.p1->getActive()) {
    testp1 = lin_s[min_e_functions.p1].second(lambda * (1 + tieeps));
    tiedp1 = (testp1 < time_p - eps);
    min_e_functions.p1->setTied(tiedp1);
  }
  if (min_e_functions.p2->getActive()) {
    testp2 = lin_s[min_e_functions.p2].second(lambda * (1 + tieeps));
    tiedp2 = (testp2 < time_p - eps);
    min_e_functions.p2->setTied(tiedp2);
  }

  // P1 ties and P2 is active - edge will go tight next
  if ((tiedp1) && (tiedp2 == false) && (min_e_functions.p2->getActive())) {
    lin_val_p1 = lin_s[min_e_functions.p1].second;
    lin_val_p2 = {
        min_e_functions.second.a - 2 * lin_s[min_e_functions.p1].second.a,
        min_e_functions.second.b - 2 * lin_s[min_e_functions.p1].second.b};
  }
  // P2 ties and P1 is active - edge will go tight next
  else if ((tiedp2) && (tiedp1 == false) && (min_e_functions.p1->getActive())) {
    lin_val_p1 = lin_s[min_e_functions.p2].second;
    lin_val_p2 = {
        min_e_functions.second.a - 2 * lin_s[min_e_functions.p2].second.a,
        min_e_functions.second.b - lin_s[min_e_functions.p2].second.b};
  }
  // P1 ties and P2 is inactive or goes neutral right after - edge between two
  // inactive components
  else if ((tiedp1) && (testp1 < testp2)) {
    alt_e = min_e;
    lin_val_p1 = {lin_val.a, lin_val.b}, lin_val_p2 = {0., 0.};
  }
  // P2 ties and P1 is inactive or goes neutral right after - edge between two
  // inactive components
  else if ((tiedp2) && (testp2 < testp1)) {
    alt_e = min_e;
    lin_val_p1 = {lin_val.a, lin_val.b}, lin_val_p2 = {0., 0.};
  }
}
