#include "grow_subsets.h"

// Linear search through subsets to find next subset which goes tight
std::pair<double, std::shared_ptr<Subset>> GrowSubsets::minSetTime() const {
  double time_s = INT_MAX;
  std::shared_ptr<Subset> min_s = nullptr;
  for (const auto& s : subsets_) {
    if (!s->getActive()) continue;

    double tight_time = lin_s_.at(s).first.t_minus;
    if (tight_time < time_s - eps_) {  // break ties by slope
      time_s = tight_time;
      min_s = s;
    }
  }
  return std::make_pair(time_s, min_s);
}

// Linear search through edges to find next edge which goes tight
std::pair<double, EdgeFunctions> GrowSubsets::minEdgeTime() const {
  double time_e = INT_MAX;
  const EdgeFunctions* optimizer = nullptr;
  for (const auto& e : edge_functions_) {
    if (!e.p1->getActive() && !e.p2->getActive()) continue;
    if (e.p1 == e.p2) continue;

    // Time to go tight for edge e
    double tight_time =
        e.first.t_minus / (int(e.p1->getActive()) + int(e.p2->getActive()));

    // If new min store constant reference (avoid copying until loop finishes)
    if (tight_time < time_e - eps_) {
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

void GrowSubsets::resolveTies(const EdgeFunctions& min_e_functions) {
  // Find event time for min_e at lambda*(1+tieeps)
  double factor = 1.0 / (int(min_e_functions.p1->getActive()) +
                         int(min_e_functions.p2->getActive()));
  double time_p = factor * min_e_functions.second.t_plus;

  // Find minimum tied edge between same subsets at lambda*(1+tieeps)
  const EdgeFunctions* optimizer = nullptr;
  for (const auto& e : edge_functions_) {
    if (((e.p1 == min_e_functions.p1) && (e.p2 == min_e_functions.p2)) ||
        ((e.p1 == min_e_functions.p2) && (e.p2 == min_e_functions.p1))) {
      // Time to go tight for edge e
      double tight_time = factor * e.second.t_plus;

      // If new min at lambda*(1+tieeps)
      if (tight_time < time_p - eps_) {
        optimizer = &e;
        time_p = tight_time;
      }
    }
  }
  if (optimizer != nullptr) {
    lin_val_p2_ = {factor * optimizer->second.t_minus,
                   factor * optimizer->second.t_plus};
    alt_e_ = optimizer->edge;
  }

  // Find if parents go neutral before the edge at lambda*(1+eps)
  double testp1 = INT_MAX, testp2 = INT_MAX;
  bool tiedp1 = false, tiedp2 = false;
  if (min_e_functions.p1->getActive()) {
    testp1 = lin_s_[min_e_functions.p1].second.t_plus;
    tiedp1 = (testp1 < time_p - eps_);
    min_e_functions.p1->setTied(tiedp1);
  }
  if (min_e_functions.p2->getActive()) {
    testp2 = lin_s_[min_e_functions.p2].second.t_plus;
    tiedp2 = (testp2 < time_p - eps_);
    min_e_functions.p2->setTied(tiedp2);
  }
  // P1 ties and P2 is active - edge will go tight next
  if ((tiedp1) && (tiedp2 == false) && (min_e_functions.p2->getActive())) {
    lin_val_p1_ = lin_s_[min_e_functions.p1].second;
    lin_val_p2_ =
        min_e_functions.second - 2 * lin_s_[min_e_functions.p1].second;
  }
  // P2 ties and P1 is active - edge will go tight next
  else if ((tiedp2) && (tiedp1 == false) && (min_e_functions.p1->getActive())) {
    lin_val_p1_ = lin_s_[min_e_functions.p2].second;

    // Project linear functions into standard coefficients and do the
    // subtraction, then project back
    const auto& e_f = min_e_functions.second;
    const auto& s_f = lin_s_[min_e_functions.p2].second;
    double a = e_f.slope(t_minus_, t_plus_) - 2 * s_f.slope(t_minus_, t_plus_);
    double b = e_f.offset(t_minus_, t_plus_) - s_f.offset(t_minus_, t_plus_);

    lin_val_p2_ = {a * t_minus_ + b, a * t_plus_ + b};
  }
  // P1 ties and P2 is inactive or goes neutral right after - edge between two
  // inactive components
  else if ((tiedp1) && (testp1 < testp2)) {
    alt_e_ = min_e_functions.edge;
    lin_val_p1_ = {lin_val_.t_minus, lin_val_.t_plus};
    lin_val_p2_ = {0., 0.};
  }
  // P2 ties and P1 is inactive or goes neutral right after - edge between two
  // inactive components
  else if ((tiedp2) && (testp2 < testp1)) {
    alt_e_ = min_e_functions.edge;
    lin_val_p1_ = {lin_val_.t_minus, lin_val_.t_plus};
    lin_val_p2_ = {0., 0.};
  }
}

// ~40% of runtime is spent in this function for large and small problems.
inline void GrowSubsets::updateEdge(EdgeFunctions* edge) {
  if (edge->p1->getActive()) edge->first -= lin_val_;
  if (edge->p2->getActive()) edge->first -= lin_val_;

  // If parent 1 is tied only raise p1 otherwise p1+p2
  if (edge->p1->getTied()) {
    edge->second -= lin_val_p1_;
  } else if (edge->p1->getActive()) {
    edge->second -= lin_val_p1_plus_p2_;
  }

  // If parent 2 is tied only raise p1 otherwise p1+p2
  if (edge->p2->getTied()) {
    edge->second -= lin_val_p1_;
  } else if (edge->p2->getActive()) {
    edge->second -= lin_val_p1_plus_p2_;
  }
}

inline void GrowSubsets::updateSubsets() {
  // Update y_vals and times to go tight
  for (auto s : subsets_) {
    if (!s->getActive()) continue;

    s->setY(s->getY() + lin_val_.t_minus);
    lin_s_[s].first -= lin_val_;

    // If tied then only raise first linear amount p1
    if (s->getTied()) {
      lin_s_[s].second -= lin_val_p1_;
    } else {
      lin_s_[s].second -= lin_val_p1_plus_p2_;
    }
  }
}

// 25% of runtime is spent in this function for small problems, ~5% for large
// problems
inline std::pair<double, EdgeFunctions>
GrowSubsets::updateEdgesGivenNeutralSubset(
    const std::shared_ptr<Subset>& min_s) {
  double time_e = INT_MAX;
  const EdgeFunctions* optimizer = nullptr;
  // Feels pretty optimized unless we go for a different data structure
  for (auto& e : edge_functions_) {
    // Part 1: Update treating min_s as active
    if (!e.p1->getActive() && !e.p2->getActive()) continue;
    updateEdge(&e);

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
      if (tight_time < time_e - eps_) {
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
inline std::pair<double, EdgeFunctions> GrowSubsets::updateEdgesGivenTightEdge(
    const std::shared_ptr<Subset>& S1, const std::shared_ptr<Subset>& S2,
    const std::shared_ptr<Subset>& S) {
  double time_e = INT_MAX;
  EdgeFunctions min_e_functions;
  for (auto it = edge_functions_.begin(); it != edge_functions_.end();) {
    // Part 1: Update edges
    if (it->p1->getActive() || it->p2->getActive()) updateEdge(&(*it));

    // Part 2: Update parents and remove if applicable
    if (it->p1 == S1 || it->p1 == S2) it->p1 = S;
    if (it->p2 == S1 || it->p2 == S2) it->p2 = S;
    if (it->p1 == it->p2) {
      // Erase by swapping with last element and popping the back
      (*it) = edge_functions_.back();
      edge_functions_.pop_back();

      // Part 3: Search for min
    } else {
      if (it->p1->getActive() || it->p2->getActive()) {
        // Evaluated at lambda * ( 1 - tieeps)
        double tight_time = it->first.t_minus / (int(it->p1->getActive()) +
                                                 int(it->p2->getActive()));

        if (tight_time < time_e - eps_) {
          time_e = tight_time;
          min_e_functions = *it;
        }
      }
      ++it;
    }
  }
  return std::make_pair(time_e, min_e_functions);
}

std::list<std::shared_ptr<Subset>> GrowSubsets::build(const Graph& G,
                                                      double lambda) {
  t_minus_ = lambda * (1 - tieeps_);
  t_plus_ = lambda * (1 + tieeps_);

  // We can represent the linear function by it's value at two known time points
  // instead of coefficients Since we exclusively operate over the time points
  // t_minus and t_plus, we might as well store those.

  // First create an active subset for each vertex and intialize a_s and b_s
  for (auto v : G.getVertices()) {
    int prize = G.getVertex(v)->getPrize();
    std::shared_ptr<Subset> Sp = std::make_shared<Subset>(v, prize, prize);
    vertex_subs_[v] = Sp;
    subsets_.push_back(Sp);
    double val_at_tminus = 0 * t_minus_ + 0.5 * prize;
    double val_at_tplus = 0 * t_plus_ + 0.5 * prize;
    lin_s_[Sp] = LinearFunctionPair{{val_at_tminus, val_at_tplus},
                                    {val_at_tminus, val_at_tplus}};
  }

  for (auto e : G.getEdges()) {
    double val_at_tminus = e->getWeight() * t_minus_ + 0.;
    double val_at_tplus = e->getWeight() * t_plus_ + 0.;
    edge_functions_.emplace_back(EdgeFunctions{e,
                                               {val_at_tminus, val_at_tplus},
                                               {val_at_tminus, val_at_tplus},
                                               vertex_subs_[e->getHead()],
                                               vertex_subs_[e->getTail()]});
  }

  auto min_edge = minEdgeTime();
  auto min_e_functions = min_edge.second;
  auto time_e = min_edge.first;

  while (true) {
    auto min_set = minSetTime();
    auto time_s = min_set.first;  // Time subset goes tight
    auto min_s = min_set.second;  // First subset to go tight

    // If nothing to go tight - then algorithm is done
    if ((min_s == nullptr) && (min_e_functions.edge == nullptr)) {
      return subsets_;
    }

    lin_val_ = LinearFunction{0, 0};
    // Update linear functions
    if (min_s == nullptr || time_e < time_s + eps_) {
      min_s = nullptr;
      double factor = 1.0 / (int(min_e_functions.p1->getActive()) +
                             int(min_e_functions.p2->getActive()));
      lin_val_ = {factor * min_e_functions.first.t_minus,
                  factor * min_e_functions.first.t_plus};
    } else {
      min_e_functions.edge = nullptr;
      lin_val_ = lin_s_.at(min_s).first;
    }
    alt_e_ = min_e_functions.edge;

    // If an edge event then we have to check for ties and update lin_vals
    lin_val_p1_ = LinearFunction{0., 0.};
    lin_val_p2_ = LinearFunction{lin_val_.t_minus, lin_val_.t_plus};
    if (min_e_functions.edge != nullptr) resolveTies(min_e_functions);

    lin_val_p1_plus_p2_ =
        LinearFunction{lin_val_p1_.t_minus + lin_val_p2_.t_minus,
                       lin_val_p1_.t_plus + lin_val_p2_.t_plus};
    updateSubsets();

    if (min_s != nullptr) {
      auto update_results = updateEdgesGivenNeutralSubset(min_s);
      time_e = update_results.first;
      min_e_functions = update_results.second;
      min_s->setActive(false);
    } else {
      auto S1 = min_e_functions.p1;
      auto S2 = min_e_functions.p2;
      std::shared_ptr<Subset> S =
          std::make_shared<Subset>(S1, S2, min_e_functions.edge, alt_e_);
      lin_s_[S].first = {lin_s_[S1].first.t_minus + lin_s_[S2].first.t_minus,
                         lin_s_[S1].first.t_plus + lin_s_[S2].first.t_plus};
      lin_s_[S].second = {lin_s_[S1].second.t_minus + lin_s_[S2].second.t_minus,
                          lin_s_[S1].second.t_plus + lin_s_[S2].second.t_plus};

      subsets_.remove(S1);
      subsets_.remove(S2);
      subsets_.push_back(S);

      // Update vertex_subs_
      for (auto v : S->getVertices()) vertex_subs_[v] = S;

      auto update_results = updateEdgesGivenTightEdge(S1, S2, S);
      time_e = update_results.first;
      min_e_functions = update_results.second;
    }
  }
}
