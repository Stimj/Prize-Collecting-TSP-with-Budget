#include "grow_subsets.h"

// Linear search through subsets to find next subset which goes tight
std::pair<double, std::shared_ptr<Subset>> minSetTime(
    const std::unordered_map<std::shared_ptr<Subset>, LinearFunctionPair>&
        lin_s,
    const std::list<std::shared_ptr<Subset>>& subsets) {
  double eps = 1.0e-15;  // Level of precision for general ties
  double time_s = INT_MAX;
  std::shared_ptr<Subset> min_s = nullptr;
  for (const auto& s : subsets) {
    if (!s->getActive()) continue;

    double tight_time = lin_s.at(s).first.t_minus;
    if (tight_time < time_s - eps) {  // break ties by slope
      time_s = tight_time;
      min_s = s;
    }
  }

  return std::make_pair(time_s, min_s);
}

// Linear search through edges to find next edge which goes tight
std::pair<double, EdgeFunctions> minEdgeTime(
    const std::vector<EdgeFunctions>& edge_functions) {
  double eps = 1.0e-15;  // Level of precision for general ties
  double time_e = INT_MAX;

  const EdgeFunctions* optimizer = nullptr;
  for (const auto& e : edge_functions) {
    if (!e.p1->getActive() && !e.p2->getActive()) continue;
    if (e.p1 == e.p2) continue;

    // Time to go tight for edge e
    double tight_time =
        e.first.t_minus / (int(e.p1->getActive()) + int(e.p2->getActive()));

    // If new min store constant reference (avoid copying until loop finishes)
    if (tight_time < time_e - eps) {
      time_e = tight_time;
      optimizer = &e;
    }
  }

  EdgeFunctions min_e_functions;
  if (optimizer != nullptr) {
    min_e_functions = *optimizer;
  }
  return std::make_pair(time_e, min_e_functions);
}

void resolveTies(
    double lambda, const std::vector<EdgeFunctions>& edge_functions,
    std::unordered_map<std::shared_ptr<Subset>, LinearFunctionPair>& lin_s,
    const EdgeFunctions& min_e_functions, std::shared_ptr<Edge>& alt_e,
    LinearFunction& lin_val, LinearFunction& lin_val_p1,
    LinearFunction& lin_val_p2) {
  double eps = 1.0e-15;   // Level of precision for general ties
  double tieeps = 0.001;  // Level of precisions to count ties
  // Find event time for min_e at lambda*(1+tieeps)
  double factor = 1.0 / (int(min_e_functions.p1->getActive()) +
                         int(min_e_functions.p2->getActive()));
  double time_p = factor * min_e_functions.second.t_plus;

  // Find minimum tied edge between same subsets at lambda*(1+tieeps)
  const EdgeFunctions* optimizer = nullptr;
  for (const auto& e : edge_functions) {
    if (((e.p1 == min_e_functions.p1) && (e.p2 == min_e_functions.p2)) ||
        ((e.p1 == min_e_functions.p2) && (e.p2 == min_e_functions.p1))) {
      // Time to go tight for edge e
      double tight_time = factor * e.second.t_plus;

      // If new min at lambda*(1+tieeps)
      if (tight_time < time_p - eps) {
        optimizer = &e;
        time_p = tight_time;
      }
    }
  }

  if (optimizer != nullptr) {
    lin_val_p2 = {factor * optimizer->second.t_minus,
                  factor * optimizer->second.t_plus};
    alt_e = optimizer->edge;
  }

  // Find if parents go neutral before the edge at lambda*(1+eps)
  double testp1 = INT_MAX, testp2 = INT_MAX;
  bool tiedp1 = false, tiedp2 = false;
  if (min_e_functions.p1->getActive()) {
    testp1 = lin_s[min_e_functions.p1].second.t_plus;
    tiedp1 = (testp1 < time_p - eps);
    min_e_functions.p1->setTied(tiedp1);
  }
  if (min_e_functions.p2->getActive()) {
    testp2 = lin_s[min_e_functions.p2].second.t_plus;
    tiedp2 = (testp2 < time_p - eps);
    min_e_functions.p2->setTied(tiedp2);
  }
  // P1 ties and P2 is active - edge will go tight next
  if ((tiedp1) && (tiedp2 == false) && (min_e_functions.p2->getActive())) {
    lin_val_p1 = lin_s[min_e_functions.p1].second;
    lin_val_p2 = min_e_functions.second - 2 * lin_s[min_e_functions.p1].second;
  }
  // P2 ties and P1 is active - edge will go tight next
  else if ((tiedp2) && (tiedp1 == false) && (min_e_functions.p1->getActive())) {
    lin_val_p1 = lin_s[min_e_functions.p2].second;

    // Project linear functions into standard coefficients and do the
    // subtraction, then project back
    auto t_minus = lambda * (1 - tieeps);
    auto t_plus = lambda * (1 + tieeps);
    const auto& e_f = min_e_functions.second;
    const auto& s_f = lin_s[min_e_functions.p2].second;
    double a = e_f.slope(t_minus, t_plus) - 2 * s_f.slope(t_minus, t_plus);
    double b = e_f.offset(t_minus, t_plus) - s_f.offset(t_minus, t_plus);

    lin_val_p2 = {a * t_minus + b, a * t_plus + b};
  }
  // P1 ties and P2 is inactive or goes neutral right after - edge between two
  // inactive components
  else if ((tiedp1) && (testp1 < testp2)) {
    alt_e = min_e_functions.edge;
    lin_val_p1 = {lin_val.t_minus, lin_val.t_plus}, lin_val_p2 = {0., 0.};
  }
  // P2 ties and P1 is inactive or goes neutral right after - edge between two
  // inactive components
  else if ((tiedp2) && (testp2 < testp1)) {
    alt_e = min_e_functions.edge;
    lin_val_p1 = {lin_val.t_minus, lin_val.t_plus}, lin_val_p2 = {0., 0.};
  }
}

// ~40% of runtime is spent in this function for large and small problems.
inline void updateEdge(const LinearFunction& lin_val,
                       const LinearFunction& lin_val_p1,
                       const LinearFunction& lin_val_p1_plus_p2,
                       EdgeFunctions* edge) {
  if (edge->p1->getActive()) edge->first -= lin_val;
  if (edge->p2->getActive()) edge->first -= lin_val;

  // If parent 1 is tied only raise p1 otherwise p1+p2
  if (edge->p1->getTied()) {
    edge->second -= lin_val_p1;
  } else if (edge->p1->getActive()) {
    edge->second -= lin_val_p1_plus_p2;
  }

  // If parent 2 is tied only raise p1 otherwise p1+p2
  if (edge->p2->getTied()) {
    edge->second -= lin_val_p1;
  } else if (edge->p2->getActive()) {
    edge->second -= lin_val_p1_plus_p2;
  }
}

inline void updateSubsets(
    const LinearFunction& lin_val, const LinearFunction& lin_val_p1,
    const LinearFunction& lin_val_p1_plus_p2,
    std::list<std::shared_ptr<Subset>>& subsets,
    std::unordered_map<std::shared_ptr<Subset>, LinearFunctionPair>& lin_s) {
  // Update y_vals and times to go tight
  for (auto s : subsets) {
    if (!s->getActive()) continue;

    s->setY(s->getY() + lin_val.t_minus);
    lin_s[s].first -= lin_val;

    // If tied then only raise first linear amount p1
    if (s->getTied()) {
      lin_s[s].second -= lin_val_p1;
    } else {
      lin_s[s].second -= lin_val_p1_plus_p2;
    }
  }
}

// 25% of runtime is spent in this function for small problems, ~5% for large
// problems
inline std::pair<double, EdgeFunctions> updateEdgesGivenNeutralSubset(
    const std::shared_ptr<Subset>& min_s, const LinearFunction& lin_val,
    const LinearFunction& lin_val_p1, const LinearFunction& lin_val_p1_plus_p2,
    std::vector<EdgeFunctions>& edge_functions) {
  double eps = 1.0e-15;  // Level of precision for general ties
  double time_e = INT_MAX;
  const EdgeFunctions* optimizer = nullptr;
  // Feels pretty optimized unless we go for a different data structure
  for (auto& e : edge_functions) {
    // Part 1: Update treating min_s as active
    if (!e.p1->getActive() && !e.p2->getActive()) continue;
    updateEdge(lin_val, lin_val_p1, lin_val_p1_plus_p2, &e);

    if (e.p1 != e.p2) {
      // Part 2: Search assuming min_s is inactive
      int factor_inverse = int(e.p1->getActive()) + int(e.p2->getActive());
      if (e.p1 == min_s || e.p2 == min_s) {
        factor_inverse -= 1;
        if (factor_inverse == 0) continue;
      }

      // Time to go tight for edge e
      double tight_time = e.first.t_minus / factor_inverse;

      // If new min store constant reference (avoid copying until loop
      // finishes)
      if (tight_time < time_e - eps) {
        time_e = tight_time;
        optimizer = &e;
      }
    }
  }

  EdgeFunctions min_e_functions;
  if (optimizer != nullptr) min_e_functions = *optimizer;

  return std::make_pair(time_e, min_e_functions);
}

// 25% of runtime is spent in this function for small problems
// 40% for large problems
inline std::pair<double, EdgeFunctions> updateEdgesGivenTightEdge(
    const std::shared_ptr<Subset>& S1, const std::shared_ptr<Subset>& S2,
    const std::shared_ptr<Subset>& S, const LinearFunction& lin_val,
    const LinearFunction& lin_val_p1, const LinearFunction& lin_val_p1_plus_p2,
    std::vector<EdgeFunctions>& edge_functions) {
  double eps = 1.0e-15;  // Level of precision for general ties
  double time_e = INT_MAX;
  EdgeFunctions min_e_functions;
  for (auto it = edge_functions.begin(); it != edge_functions.end();) {
    // Part 1: Update edges
    // This should only happen on active edges, but it doesn't seem to affect
    // performance?
    if (it->p1->getActive() || it->p2->getActive())
      updateEdge(lin_val, lin_val_p1, lin_val_p1_plus_p2, &(*it));

    // Part 2: Update parents and remove if applicable
    if ((*it).p1 == S1 || (*it).p1 == S2) (*it).p1 = S;
    if ((*it).p2 == S1 || (*it).p2 == S2) (*it).p2 = S;
    if ((*it).p1 == (*it).p2) {
      // Erase by swapping with last element and popping the back
      (*it) = edge_functions.back();
      edge_functions.pop_back();

      // Part 3: Search for min
    } else {
      if ((*it).p1->getActive() || (*it).p2->getActive()) {
        // Evaluated at lambda * ( 1 - tieeps)
        double tight_time = (*it).first.t_minus / (int((*it).p1->getActive()) +
                                                   int((*it).p2->getActive()));

        if (tight_time < time_e - eps) {
          time_e = tight_time;
          min_e_functions = *it;
        }
      }
      ++it;
    }
  }
  return std::make_pair(time_e, min_e_functions);
}

// Grow Function
std::list<std::shared_ptr<Subset>> growSubsets(const Graph& G, double lambda) {
  double tieeps = 0.001;  // Level of precisions to count ties
  double eps = 1.0e-15;   // Level of precision for general ties
  const std::list<int>& V = G.getVertices();  // vertices in G

  std::list<std::shared_ptr<Subset>> subsets;  // list current subsets
  std::unordered_map<int, std::shared_ptr<Subset>>
      vertexSubs;  // unordered_map vertices to subset that contains them

  // Time to go tight is alpha*lambda+beta
  std::unordered_map<std::shared_ptr<Subset>, LinearFunctionPair>
      lin_s;  // a and b values for subsets

  double t_minus = lambda * (1 - tieeps);
  double t_plus = lambda * (1 + tieeps);
  // We can represent the linear function by it's value at two known time points
  // instead of coefficients Since we exclusively operate over the time points
  // t_minus and t_plus, we might as well store those.

  // First create an active subset for each vertex and intialize a_s and b_s
  for (auto v : V) {
    int prize = G.getVertex(v)->getPrize();
    std::shared_ptr<Subset> Sp = std::make_shared<Subset>(v, prize, prize);
    vertexSubs[v] = Sp;
    subsets.push_back(Sp);
    double val_at_tminus = 0 * t_minus + 0.5 * prize;
    double val_at_tplus = 0 * t_plus + 0.5 * prize;
    lin_s[Sp] = LinearFunctionPair{{val_at_tminus, val_at_tplus},
                                   {val_at_tminus, val_at_tplus}};
  }

  std::vector<EdgeFunctions> edge_functions;
  for (auto e : G.getEdges()) {
    double val_at_tminus = e->getWeight() * t_minus + 0.;
    double val_at_tplus = e->getWeight() * t_plus + 0.;
    edge_functions.emplace_back(EdgeFunctions{e,
                                              {val_at_tminus, val_at_tplus},
                                              {val_at_tminus, val_at_tplus},
                                              vertexSubs[e->getHead()],
                                              vertexSubs[e->getTail()]});
  }

  auto min_edge = minEdgeTime(edge_functions);
  auto min_e_functions = min_edge.second;
  auto time_e = min_edge.first;

  while (true) {
    // TODO: use structured bindings?
    auto min_set = minSetTime(lin_s, subsets);
    auto time_s = min_set.first;  // Time subset goes tight
    auto min_s = min_set.second;  // First subset to go tight

    // If nothing to go tight - then algorithm is done
    if ((min_s == nullptr) && (min_e_functions.edge == nullptr)) {
      return subsets;
    }

    // TODO: clean these names up
    LinearFunction lin_val{0, 0};  // Linear functions for time to go tight
    // Update linear functions
    if (min_s == nullptr || time_e < time_s + eps) {
      min_s = nullptr;
      double factor = 1.0 / (int(min_e_functions.p1->getActive()) +
                             int(min_e_functions.p2->getActive()));
      lin_val = {factor * min_e_functions.first.t_minus,
                 factor * min_e_functions.first.t_plus};
    } else {
      min_e_functions.edge = nullptr;
      lin_val = lin_s.at(min_s).first;
    }
    auto alt_e = min_e_functions.edge;

    // If an edge event then we have to check for ties and update lin_vals
    LinearFunction lin_val_p1{0., 0.};
    LinearFunction lin_val_p2{lin_val.t_minus, lin_val.t_plus};
    if (min_e_functions.edge != nullptr) {
      resolveTies(lambda, edge_functions, lin_s, min_e_functions, alt_e,
                  lin_val, lin_val_p1, lin_val_p2);
    }

    LinearFunction lin_val_p1_plus_p2{lin_val_p1.t_minus + lin_val_p2.t_minus,
                                      lin_val_p1.t_plus + lin_val_p2.t_plus};
    updateSubsets(lin_val, lin_val_p1, lin_val_p1_plus_p2, subsets, lin_s);

    if (min_s != nullptr) {
      auto update_results = updateEdgesGivenNeutralSubset(
          min_s, lin_val, lin_val_p1, lin_val_p1_plus_p2, edge_functions);
      time_e = update_results.first;
      min_e_functions = update_results.second;
      min_s->setActive(false);
    } else {
      auto S1 = min_e_functions.p1;
      auto S2 = min_e_functions.p2;
      std::shared_ptr<Subset> S =
          std::make_shared<Subset>(S1, S2, min_e_functions.edge, alt_e);
      lin_s[S].first = {lin_s[S1].first.t_minus + lin_s[S2].first.t_minus,
                        lin_s[S1].first.t_plus + lin_s[S2].first.t_plus};
      lin_s[S].second = {lin_s[S1].second.t_minus + lin_s[S2].second.t_minus,
                         lin_s[S1].second.t_plus + lin_s[S2].second.t_plus};

      subsets.remove(S1);
      subsets.remove(S2);
      subsets.push_back(S);

      // Update vertexSubs
      const std::list<int>& vinS = S->getVertices();
      for (auto v : vinS) {
        vertexSubs[v] = S;
      }

      auto update_results = updateEdgesGivenTightEdge(
          S1, S2, S, lin_val, lin_val_p1, lin_val_p1_plus_p2, edge_functions);
      time_e = update_results.first;
      min_e_functions = update_results.second;
    }
  }
}
