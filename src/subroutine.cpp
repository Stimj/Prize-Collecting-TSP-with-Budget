//
//  Subroutine.cpp
//  Prize-Collecting TSP
//
//  Created by Alice Paul on 3/14/17.
//
//  MIT License
//  Copyright (c) 2020 alicepaul
//

#include "subroutine.h"

#include <chrono>

/* ------------------------- HELPER FUNCTIONS --------------------------*/

struct CheapTimer {
  std::chrono::time_point<std::chrono::high_resolution_clock> t0 =
      std::chrono::high_resolution_clock::now();

  int mark() {
    int micros = std::chrono::duration_cast<std::chrono::microseconds>(
                     std::chrono::high_resolution_clock::now() - t0)
                     .count();
    t0 = std::chrono::high_resolution_clock::now();
    return micros;
  }
};

/* ------------------------- MAIN FUNCTIONS --------------------------*/

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

  // First create an active subset for each vertex and intialize a_s and b_s
  for (auto v : V) {
    int prize = G.getVertex(v)->getPrize();
    std::shared_ptr<Subset> Sp = std::make_shared<Subset>(v, prize, prize);
    vertexSubs[v] = Sp;
    subsets.push_back(Sp);
    lin_s[Sp] = LinearFunctionPair{{0, 0.5 * prize},
                                   {0, 0.5 * prize}};  // time_s = 0*lambda+0.5
  }

  std::vector<EdgeFunctions> edge_functions;
  // TODO: Consider replacing linear functions with time_tight and time_tight_p
  // They need to be updated if either:
  // 1) subset membership changes
  // 2) parent subset becomes active
  // 3) Linear function is updated
  // The key insight here is that we can represent the linear function by it's
  // value at two known time points instead of coefficients Since we exclusively
  // operate over the time points, we might as well store those.
  for (auto e : G.getEdges()) {
    edge_functions.emplace_back(
        EdgeFunctions{e,
                      {e->getWeight(), 0.},  // TODO: time_tight
                      {e->getWeight(), 0},   // TODO: time_tight_p
                      vertexSubs[e->getHead()],
                      vertexSubs[e->getTail()]});
  }

  auto min_edge = minEdgeTime(edge_functions, lambda * (1-tieeps), INT_MAX);
  auto min_e_functions = min_edge.second;
  auto time_e = min_edge.first;

  while (true) {

    // Find next event and check if finished //
    std::shared_ptr<Subset> min_s = nullptr;  // First subset to go tight
    std::shared_ptr<Edge> alt_e =
        nullptr;                   // First edge to go tight and alt edge
    LinearFunction lin_val{0, 0};  // Linear functions for time to go tight
    LinearFunction lin_val_p1{0, 0};
    LinearFunction lin_val_p2{0, 0};  // Linear functions to go tight at lambda+

    auto min_set = minSetTime(lin_s, subsets, lambda * (1 - tieeps));
    auto time_s = min_set.first;
    min_s = min_set.second;
    if (min_s == nullptr) time_s = INT_MAX;

    // If nothing to go tight - then algorithm is done
    if ((min_s == nullptr) && (min_e_functions.edge == nullptr)) {
      return subsets;
    }


    // Update linear functions
    if (min_s == nullptr || time_e < time_s + eps) {
      min_s = nullptr;
      double factor = 1.0 / (int(min_e_functions.p1->getActive()) +
                             int(min_e_functions.p2->getActive()));
      lin_val = {factor * min_e_functions.first.a,
                 factor * min_e_functions.first.b};
    } else {
      min_e_functions.edge = nullptr;
      lin_val = lin_s.at(min_s).first;
    }

    // NOTE: this will need to convert to appropriate time stamp
    lin_val_p1 = {0., 0.}, lin_val_p2 = {lin_val.a, lin_val.b};
    alt_e = min_e_functions.edge;

    // If an edge event then we have to check for ties
    if (min_e_functions.edge != nullptr) {
      resolveTies(lambda, edge_functions, lin_s, min_e_functions, alt_e,
                  lin_val, lin_val_p1, lin_val_p2);
    }

    // Update y_vals and times to go tight
    for (auto s : subsets) {
      if (s->getActive()) {
        s->setY(s->getY() + lin_val(lambda * (1 - tieeps)));
        lin_s[s].first.a -= lin_val.a, lin_s[s].first.b -= lin_val.b;

        // If tied then only raise first linear amount p1
        if (s->getTied()) {
          lin_s[s].second.a -= lin_val_p1.a, lin_s[s].second.b -= lin_val_p1.b;
        } else {
          lin_s[s].second.a -= (lin_val_p1.a + lin_val_p2.a);
          lin_s[s].second.b -= (lin_val_p1.b + lin_val_p2.b);
        }
      }
    }

    LinearFunction lin_val_p1_plus_p2;
    lin_val_p1_plus_p2.a = lin_val_p1.a + lin_val_p2.a;
    lin_val_p1_plus_p2.b = lin_val_p1.b + lin_val_p2.b;
    // NOTE:
    // modifying coefficients of a linear function is equivalent to modifying
    // it's pair of time points "first" is evaluated at lambda * (1-tieeps) in
    // both of the min-search loops. "second" is evaluated at lambda * (1 +
    // tieeps) in the tie-breaking loop The challenge is that lin_val,
    // lin_val_p1, lin_val_p2 may come from edge functions One approach: Store
    // linear functions as a function of time parametrized by relevant times So
    // first.a is the function evaluated at time lambda * (1-tieeps),
    //   second.a is the function evaluated at time lambda * (1 + tieeps)
    // lin_val will be stored according to time lambda * ( 1 - tieeps)
    // lin_val_p1/lin_val_p2 will be stored according to time lambda * (1 +
    // tieeps)
    //
    // Now we've eliminated the need to evaluate the linear functions.
    //
    // At a minimum, this loop can remember the earliest event, and can store
    // use that as a lower bound Whenever subset activity or membership is
    // changed, we need to re-evaluate the times. NOTE: Update linear functions
    // of currently active edges. Affects min_e search. This loop is exhaustive
    // reinitialize the search for min_edge

    // TODO: break this into separate update functions. This is hard to follow currently.
    // If subset goes neutral first - mark inactive
    if (min_s != nullptr) {
      time_e = INT_MAX;
      min_e_functions.edge = nullptr;
      const EdgeFunctions* optimizer = nullptr;

      // This gets evaluated ~20M times for a 100 node graph.
      // Feels pretty optimized unless we go for a different data structure
      for (auto& e : edge_functions) {
        // Part 1: Update treating min_s as active
        if (!e.p1->getActive() && !e.p2->getActive()) continue;
        // Equals 2 if both are active, 1 if one is active.
        int factor_inverse = int(e.p1->getActive()) + int(e.p2->getActive());
        e.first.a -= lin_val.a * factor_inverse;
        e.first.b -= lin_val.b * factor_inverse;

        // If parent 1 is tied only raise p1 otherwise p1+p2
        if (e.p1->getTied()) {
          e.second.a -= lin_val_p1.a;
          e.second.b -= lin_val_p1.b;
        } else if (e.p1->getActive()) {
          e.second.a -= lin_val_p1_plus_p2.a;
          e.second.b -= lin_val_p1_plus_p2.b;
        }

        // If parent 2 is tied only raise p1 otherwise p1+p2
        if (e.p2->getTied()) {
          e.second.a -= lin_val_p1.a;
          e.second.b -= lin_val_p1.b;
        } else if (e.p2->getActive()) {
          e.second.a -= lin_val_p1_plus_p2.a;
          e.second.b -= lin_val_p1_plus_p2.b;
        }

        if (e.p1 != e.p2) {
          // Part 2: Search assuming min_s is inactive
          if (e.p1 == min_s || e.p2 == min_s) {
            factor_inverse -= 1;
            if (factor_inverse == 0) continue;
          }

          // Time to go tight for edge e
          double tight_time = e.first(lambda * (1 - tieeps)) / factor_inverse;

          // If new min store constant reference (avoid copying until loop
          // finishes)
          if (tight_time < time_e - eps) {
            time_e = tight_time;
            optimizer = &e;
          }
        }
      }

      if (optimizer != nullptr) {
        if (optimizer->edge == nullptr) {
          std::cout << "optmizer found nullptr!\n";
        }
        min_e_functions.edge = optimizer->edge;
        min_e_functions.first = optimizer->first;
        min_e_functions.second = optimizer->second;
        min_e_functions.p1 = optimizer->p1;
        min_e_functions.p2 = optimizer->p2;
      }

      min_s->setActive(false);
    }
    // Else an edge goes tight first - merge subsets
    else {

      std::shared_ptr<Subset> S1 = min_e_functions.p1;
      std::shared_ptr<Subset> S2 = min_e_functions.p2;
      std::shared_ptr<Subset> S =
          std::make_shared<Subset>(S1, S2, min_e_functions.edge, alt_e);
      lin_s[S].first = {lin_s[S1].first.a + lin_s[S2].first.a,
                        lin_s[S1].first.b + lin_s[S2].first.b};
      lin_s[S].second = {lin_s[S1].second.a + lin_s[S2].second.a,
                         lin_s[S1].second.b + lin_s[S2].second.b};

      subsets.remove(S1);
      subsets.remove(S2);
      subsets.push_back(S);

      // Update vertexSubs
      const std::list<int>& vinS = S->getVertices();
      for (auto v : vinS) {
        vertexSubs[v] = S;
      }

      // Update parents and erase all redundant edges. Store the minimum element.
      time_e = INT_MAX;
      min_e_functions.edge = nullptr;
      const EdgeFunctions* optimizer = nullptr;
      for (auto it = edge_functions.begin(); it != edge_functions.end();) {
        // Part 1: Update treating min_s as active
        if (it->p1->getActive() || it->p2->getActive()) {
          // Equals 2 if both are active, 1 if one is active.
          int factor_inverse = int(it->p1->getActive()) + int(it->p2->getActive());
          it->first.a -= lin_val.a * factor_inverse;
          it->first.b -= lin_val.b * factor_inverse;
          // If parent 1 is tied only raise p1 otherwise p1+p2
          if (it->p1->getTied()) {
            it->second.a -= lin_val_p1.a;
            it->second.b -= lin_val_p1.b;
          } else if (it->p1->getActive()) {
            it->second.a -= lin_val_p1_plus_p2.a;
            it->second.b -= lin_val_p1_plus_p2.b;
          }

          // If parent 2 is tied only raise p1 otherwise p1+p2
          if (it->p2->getTied()) {
            it->second.a -= lin_val_p1.a;
            it->second.b -= lin_val_p1.b;
          } else if (it->p2->getActive()) {
            it->second.a -= lin_val_p1_plus_p2.a;
            it->second.b -= lin_val_p1_plus_p2.b;
          }
        }

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
            // Time to go tight for edge e
            double tight_time =
                (*it).first(lambda * (1 - tieeps)) /
                (int((*it).p1->getActive()) + int((*it).p2->getActive()));

            // If new min store constant reference (avoid copying until loop
            // finishes)
            if (tight_time < time_e - eps) {
              time_e = tight_time;
              min_e_functions = *it;
            }
          }
          ++it;
        }
      }
    }
  }
}

// Prune Function
double prune(std::shared_ptr<Subset>& s,
             std::list<std::shared_ptr<Edge>>& edges, bool l_plus, bool swap) {
  // Base case is that s does not have a parent
  if (s->getParent1() == nullptr) {
    return 0;
  }

  // Otherwise find parents and number of endpoints
  std::shared_ptr<Subset> p1 = s->getParent1(), p2 = s->getParent2();
  std::shared_ptr<Edge> e = s->getEdge(), alt_e = s->getAltEdge();
  if ((l_plus == true) && (alt_e != nullptr) && (swap == true)) {
    e = alt_e;
  }

  // Find degree of p1 given current edges
  int connected1 = 0;
  for (auto v : p1->getVertices()) {
    for (auto e : edges) {
      if ((e->getHead() == v) || (e->getTail() == v)) connected1 += 1;
    }
  }

  // Find degree of p2 given current edges
  int connected2 = 0;
  for (auto v : p2->getVertices()) {
    for (auto e : edges) {
      if ((e->getHead() == v) || (e->getTail() == v)) connected2 += 1;
    }
  }

  // Find if active. If l_plus = true don't include those that tied
  bool p1active = p1->getActive();
  bool p2active = p2->getActive();
  if (l_plus) {
    if (p1->getTied()) {
      p1active = false;
    }
    if (p2->getTied()) {
      p2active = false;
    }
  }

  // If p1 and p2 are active or neither needs to be pruned, add edge between and
  // recurse
  if (((p1active && p2active) || (p1active && connected2)) ||
      (p2active && connected1) || ((connected1 > 0) && (connected2 > 0))) {
    edges.push_back(e);
    double w1 = prune(p1, edges, l_plus, swap);
    double w2 = prune(p2, edges, l_plus, swap);
    return w1 + w2 + e->getWeight();
  }

  // Else if p1 is active or does need to be pruned, recurse on p1
  else if ((p1active) || (connected1 > 1)) {
    // std::cout << "Prune " << *p2;
    return prune(p1, edges, l_plus, swap);
  }

  // Else if p2 is active or does need to be pruned, recurse on p2
  else if ((p2active) || (connected2 > 1)) {
    // std::cout << "Prune " << *p1;
    return prune(p2, edges, l_plus, swap);
  }

  // Else if no edges are added yet - find largest and return
  else if (edges.size() == 0) {
    std::list<std::shared_ptr<Edge>> edges1, edges2;
    double w1 = prune(p1, edges1, l_plus, swap),
           w2 = prune(p2, edges2, l_plus, swap);
    if (w1 > w2) {
      edges = edges1;
      return w1;
    } else {
      edges = edges2;
      return w2;
    }
  }

  return 0;
}

// Find path of maximal pruned subsets that are not pruned when s is marked
// active
void maxPrunedS(std::shared_ptr<Subset>& s,
                std::list<std::shared_ptr<Edge>>& edges,
                std::shared_ptr<Subset>& tied_S,
                std::vector<std::shared_ptr<Subset>>& prunedS,
                std::vector<std::shared_ptr<Edge>>& prunedE) {
  // Base case is that s does not have a parent
  if (s->getParent1() == nullptr) {
    return;
  }

  // Otherwise find parents and number of endpoints
  std::shared_ptr<Subset> p1 = s->getParent1(), p2 = s->getParent2();
  int u = tied_S->getVertices().back();

  // Find if p1 has another incident edge already included and searched pruned
  // edges
  int connected1 = 0, connectedTied1 = 0;
  std::shared_ptr<Edge> connectedE1 = nullptr;
  bool inp1 = false;
  for (auto v : p1->getVertices()) {
    for (auto e : edges) {
      if ((e->getHead() == v) || (e->getTail() == v)) {
        connectedE1 = e;
        connected1 += 1;
      }
    }
    if (v == u) inp1 = true;
    for (auto e : prunedE) {
      if ((e->getHead() == v) || (e->getTail() == v)) connectedTied1 += 1;
    }
  }

  // Find if p2 has another incident edge already included and search pruned
  // edges
  int connected2 = 0, connectedTied2 = 0;
  std::shared_ptr<Edge> connectedE2 = nullptr;
  bool inp2 = false;
  for (auto v : p2->getVertices()) {
    for (auto e : edges) {
      if ((e->getHead() == v) || (e->getTail() == v)) {
        connectedE2 = e;
        connected2 += 1;
      }
    }
    if (v == u) inp2 = true;
    for (auto e : prunedE) {
      if ((e->getHead() == v) || (e->getTail() == v)) connectedTied2 += 1;
    }
  }

  // Find if active. If l_plus = true don't include those that tied
  bool p1active = p1->getActive();
  bool p2active = p2->getActive();

  // If p1 and p2 are active or neither needs to be pruned, add edge between and
  // recurse
  if (((p1active && p2active) || (p1active && connected2)) ||
      (p2active && connected1) || ((connected1 > 0) && (connected2 > 0))) {
    // std::cout << "Case 1 \n";
    // std::cout << "Edge added " << *(s->getEdge()) << "\n";
    edges.push_back(s->getEdge());
    maxPrunedS(p1, edges, tied_S, prunedS, prunedE);
    maxPrunedS(p2, edges, tied_S, prunedS, prunedE);
  }

  // Else if p1 is active, recurse on p1 - check whether p2 would have been
  // pruned in other case
  else if ((p1active) || (connected1 > 1)) {
    // std::cout << "Case 2 \n";
    if ((connectedTied2 > 0) || (tied_S == p2)) {
      prunedE.insert(prunedE.begin(), s->getEdge());
      prunedS.insert(prunedS.begin(), p2);
    }
    maxPrunedS(p1, edges, tied_S, prunedS, prunedE);
  }

  // Else if p2 is active, recurse on p2 - check whether p1 would have been
  // pruned in other case
  else if ((p2active) || (connected2 > 1)) {
    // std::cout << "Case 3 \n";
    if ((connectedTied1 > 0) || (tied_S == p1)) {
      prunedE.insert(prunedE.begin(), s->getEdge());
      prunedS.insert(prunedS.begin(), p1);
    }
    maxPrunedS(p2, edges, tied_S, prunedS, prunedE);
  }

  // Else if p1 == tied_S and connected by a single edge - would have kept
  // otherwise
  else if ((p1 == tied_S) && (connected1 == 1)) {
    // std::cout << "Case 4 \n";
    prunedE.insert(prunedE.begin(), connectedE1);
    prunedS.insert(prunedS.begin(), p1);
  }

  // Else if p1 == tied_S and p2 is inactive and connected by a single edge
  else if ((p1 == tied_S) && (connected2 == 1)) {
    // std::cout << "Case 5 \n";
    prunedE.push_back(connectedE2);
    prunedS.push_back(p2);
    prunedE.push_back(s->getEdge());
    prunedS.push_back(p1);
  }

  // Else if p1 == tied_S and connected by a single edge - would have kept
  // otherwise
  else if ((p2 == tied_S) && (connected2 == 1)) {
    // std::cout << "Case 6 \n";
    prunedE.insert(prunedE.begin(), connectedE2);
    prunedS.insert(prunedS.begin(), p2);
  }

  // Else if p2 == tied_S and p1 is inactive and connected by a single edge
  else if ((p2 == tied_S) && (connected1 == 1)) {
    // std::cout << "Case 7 \n";
    prunedE.push_back(connectedE1);
    prunedS.push_back(p1);
    prunedE.push_back(s->getEdge());
    prunedS.push_back(p2);
  }

  // Else if both p1 and p2 are inactive then we need to find which one contains
  // test_s
  else if (inp1) {
    maxPrunedS(p1, edges, tied_S, prunedS, prunedE);
  } else if (inp2) {
    maxPrunedS(p2, edges, tied_S, prunedS, prunedE);
  }
}

// Find path of maximal pruned subsets that are not pruned when s uses alt edge
void maxPrunedE(std::shared_ptr<Subset>& s,
                std::list<std::shared_ptr<Edge>>& edges,
                std::shared_ptr<Subset>& tied_S,
                std::vector<std::shared_ptr<Subset>>& prunedS,
                std::vector<std::shared_ptr<Edge>>& prunedE) {
  // Base case is that s does not have a parent
  if (s->getParent1() == nullptr) {
    return;
  }

  // Otherwise find parents and number of endpoints
  std::shared_ptr<Subset> p1 = s->getParent1(), p2 = s->getParent2();
  std::shared_ptr<Edge> alt_e = tied_S->getAltEdge();
  int u = tied_S->getVertices().back();

  // Find if p1 has another incident edge already included and searched pruned
  // edges
  int connected1 = 0, connectedTied1 = 0;
  bool inp1 = false;
  for (auto v : p1->getVertices()) {
    for (auto e : edges) {
      if ((e->getHead() == v) || (e->getTail() == v)) connected1 += 1;
    }
    if (v == u) inp1 = true;
    for (auto e : prunedE) {
      if ((e->getHead() == v) || (e->getTail() == v)) connectedTied1 += 1;
    }
    if ((alt_e->getHead() == v) || (alt_e->getTail() == v)) connectedTied1 += 1;
  }

  // Find if p2 has another incident edge already included and search pruned
  // edges plus the alt edge
  int connected2 = 0, connectedTied2 = 0;
  bool inp2 = false;
  for (auto v : p2->getVertices()) {
    for (auto e : edges) {
      if ((e->getHead() == v) || (e->getTail() == v)) connected2 += 1;
    }
    if (v == u) inp2 = true;
    for (auto e : prunedE) {
      if ((e->getHead() == v) || (e->getTail() == v)) connectedTied2 += 1;
    }
    if ((alt_e->getHead() == v) || (alt_e->getTail() == v)) connectedTied2 += 1;
  }

  // Find if active. If l_plus = true don't include those that tied
  bool p1active = p1->getActive();
  bool p2active = p2->getActive();

  // If s == tied_S then keep e and recurse on both parents
  if (s == tied_S) {
    edges.push_back(s->getEdge());
    maxPrunedE(p1, edges, tied_S, prunedS, prunedE);
    prunedE.push_back(alt_e);  // Push alt_e onto back of path then continue
    maxPrunedE(p2, edges, tied_S, prunedS, prunedE);
  }

  // If p1 and p2 are active or neither needs to be pruned, add edge between and
  // recurse
  else if (((p1active && p2active) || (p1active && connected2)) ||
           (p2active && connected1) || ((connected1 > 0) && (connected2 > 0))) {
    edges.push_back(s->getEdge());
    maxPrunedE(p1, edges, tied_S, prunedS, prunedE);
    maxPrunedE(p2, edges, tied_S, prunedS, prunedE);
  }

  // Else if p1 is active, recurse on p1 - check whether p2 would have been
  // pruned in other case
  else if ((p1active) || (connected1 > 1)) {
    if (connectedTied2 > 0) {
      prunedE.push_back(s->getEdge());
      prunedS.push_back(p2);
    }
    maxPrunedE(p1, edges, tied_S, prunedS, prunedE);
  }

  // Else if p2 is active, recurse on p2 - check whether p1 would have been
  // pruned in other case
  else if ((p2active) || (connected2 > 1)) {
    if (connectedTied1 > 0) {
      prunedE.push_back(s->getEdge());
      prunedS.push_back(p1);
    }
    maxPrunedE(p2, edges, tied_S, prunedS, prunedE);
  }

  // Else if both p1 and p2 are inactive then we need to find which one contains
  // test_s
  else if (inp1) {
    maxPrunedE(p1, edges, tied_S, prunedS, prunedE);
  } else if (inp2) {
    maxPrunedE(p2, edges, tied_S, prunedS, prunedE);
  }
}

// Reverse Delete Function
double reverseDelete(std::list<std::shared_ptr<Subset>>& subsets,
                     std::list<std::shared_ptr<Edge>>& edges,
                     std::shared_ptr<Subset>& largestSub, bool l_plus,
                     bool swap_edges) {
  // Iterate through components to prune and return the largest
  double largest = -INT_MAX;
  largestSub = nullptr;
  for (auto s : subsets) {
    std::list<std::shared_ptr<Edge>> edgesS;
    double sizeComp = prune(s, edgesS, l_plus, swap_edges);
    if (sizeComp > largest) {
      largestSub = s;
      largest = sizeComp;
      edges = edgesS;
    }
  }
  return largest;
}

/* ------------------------- ALTERNATE FUNCTIONS --------------------------*/

double reverseDelete(std::list<std::shared_ptr<Subset>>& subsets, bool l_plus,
                     bool swap_edges) {
  std::list<std::shared_ptr<Edge>> edges;
  std::shared_ptr<Subset> largestSub = nullptr;
  return reverseDelete(subsets, edges, largestSub, l_plus, swap_edges);
}

double reverseDelete(std::shared_ptr<Subset>& s,
                     std::list<std::shared_ptr<Edge>>& edges, bool l_plus,
                     bool swap_edges) {
  double w = prune(s, edges, l_plus, swap_edges);
  return w;
}

double reverseDelete(std::shared_ptr<Subset>& s, bool l_plus, bool swap_edges) {
  std::list<std::shared_ptr<Edge>> edges;
  return reverseDelete(s, edges, l_plus, swap_edges);
}

