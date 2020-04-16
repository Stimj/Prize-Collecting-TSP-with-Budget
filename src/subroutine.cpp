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

double eps = 1.0e-15;   // Level of precision for general ties
double tieeps = 0.001;  // Level of precisions to count ties

/* ------------------------- HELPER FUNCTIONS --------------------------*/

struct EdgeFunctions {
  std::shared_ptr<Edge> edge;
  LinearFunction first;
  LinearFunction second;
  std::shared_ptr<Subset> p1;
  std::shared_ptr<Subset> p2;
};

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

// Find min event to occur next
void findMinEvent(
    double lambda, const std::list<std::shared_ptr<Subset>>& subsets,
    std::vector<EdgeFunctions>& edge_functions,
    std::unordered_map<std::shared_ptr<Subset>, LinearFunctionPair>& lin_s,
    std::shared_ptr<Subset>& min_s, std::shared_ptr<Edge>& min_e,
    std::shared_ptr<Edge>& alt_e, LinearFunction& lin_val,
    LinearFunction& lin_val_p1, LinearFunction& lin_val_p2) {

  // Find minimum event between active subsets and active edges
  // Iterate through active subsets
  min_e = nullptr;
  min_s = nullptr;
  double time_s = INT_MAX;
  double time_e = INT_MAX;
  for (auto s : subsets) {
    if (!s->getActive()) continue;

    double tight_time = lin_s[s].first(lambda * (1 - tieeps));
    if (tight_time < time_s - eps) {  // break ties by slope
      time_s = tight_time;
      lin_val = lin_s[s].first;
      min_s = s;
    }
  }

  // Iterate through active edges
  EdgeFunctions min_e_functions;
  for (auto e : edge_functions) {
    if (e.p1 == e.p2) continue;
    if (!e.p1->getActive() && !e.p2->getActive()) continue;

    // Time to go tight for edge e
    double factor = 1.0 / (int(e.p1->getActive()) + int(e.p2->getActive()));
    double tight_time = factor * e.first(lambda * (1 - tieeps));

    // If new min
    if (tight_time < time_s + eps && tight_time < time_e - eps) {
      time_e = tight_time;
      lin_val = {factor * e.first.a, factor * e.first.b};
      min_e_functions = e;
      min_e = e.edge;
    }
  }

  // Find which event is first and which tied
  if (min_e != nullptr) min_s = nullptr;

  // If an edge event then we have to check for ties
  lin_val_p1 = {0., 0.}, lin_val_p2 = {lin_val.a, lin_val.b};
  alt_e = min_e;
  if (min_e != nullptr) {
    // Find event time for min_e at lambda*(1+tieeps)
    double factor = 1.0 / (int(min_e_functions.p1->getActive()) +
                           int(min_e_functions.p2->getActive()));
    double time_p = factor * min_e_functions.second(lambda * (1 + tieeps));

    // Find minimum tied edge between same subsets at lambda*(1+tieeps)
    for (auto e : edge_functions) {
      if (((e.p1 == min_e_functions.p1) && (e.p2 == min_e_functions.p2)) ||
          ((e.p1 == min_e_functions.p2) && (e.p2 == min_e_functions.p1))) {
        // Time to go tight for edge e
        double tight_time = factor * e.second(lambda * (1 + tieeps));

        // If new min at lambda*(1+tieeps)
        if (tight_time < time_p - eps) {
          lin_val_p2 = {factor * e.second.a, factor * e.second.b};
          alt_e = e.edge;
          time_p = tight_time;
        }
      }
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
    else if ((tiedp2) && (tiedp1 == false) &&
             (min_e_functions.p1->getActive())) {
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
}

/* ------------------------- MAIN FUNCTIONS --------------------------*/

// Grow Function
std::list<std::shared_ptr<Subset>> growSubsets(const Graph& G, double lambda) {
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
  for (auto e : G.getEdges()) {
    edge_functions.emplace_back(EdgeFunctions{e,
                                              {e->getWeight(), 0.},
                                              {e->getWeight(), 0},
                                              vertexSubs[e->getHead()],
                                              vertexSubs[e->getTail()]});
  }

  while (true) {
    // std::cout << "---------Next iteration-----------\n";

    // Find next event
    std::shared_ptr<Subset> min_s = nullptr;  // First subset to go tight
    std::shared_ptr<Edge> min_e = nullptr,
                          alt_e =
                              nullptr;  // First edge to go tight and alt edge
    LinearFunction lin_val{0, 0};       // Linear functions for time to go tight
    LinearFunction lin_val_p1{0, 0};
    LinearFunction lin_val_p2{0, 0};  // Linear functions to go tight at lambda+
    findMinEvent(lambda, subsets, edge_functions, lin_s, min_s, min_e, alt_e,
                 lin_val, lin_val_p1, lin_val_p2);

    // If nothing to go tight - then algorithm is done
    if ((min_s == nullptr) && (min_e == nullptr)) {
      //     auto t_total = static_cast<double>(t_init + t_find_min + t_yvals +
      //     t_active_edges + t_merge_subsets);
      //      std::cout << "getSubsets: init: " << t_init / t_total << "
      //      find_min: " << t_find_min/t_total << " yvals: " << t_yvals/t_total
      //      << " active_edges: " << t_active_edges / t_total << "
      //      merge_subsets: " << t_merge_subsets / t_total << std::endl;
      // std::cout ///
      return subsets;
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
    for (auto& e : edge_functions) {
      // This gets evaluated ~20M times for a 100 node graph.
      // Feels pretty optimized unless we go for a different data structure
      if (e.p1->getActive() || e.p2->getActive()) {
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
      }
    }

    // std::cout << "------------ EVENT ------------ \n";

    // If subset goes neutral first - mark inactive
    if (min_s != nullptr) {
      // std::cout << " Subset Event " << *min_s;
      min_s->setActive(false);
    }

    // Else an edge goes tight first - merge subsets
    else {
      // if (min_e != alt_e)
      //    std::cout << " Edge Event " << *min_e  << " with alt edge " <<
      //    *alt_e << "\n";
      // else
      //    std::cout << "Edge Event " << *min_e << "\n";
      std::shared_ptr<Subset> S1 = vertexSubs[min_e->getHead()];
      std::shared_ptr<Subset> S2 = vertexSubs[min_e->getTail()];
      std::shared_ptr<Subset> S =
          std::make_shared<Subset>(S1, S2, min_e, alt_e);
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

      // Update parents and erase all redundant edges
      for (auto it = edge_functions.begin(); it != edge_functions.end();) {
        if ((*it).p1 == S1 || (*it).p1 == S2) (*it).p1 = S;
        if ((*it).p2 == S1 || (*it).p2 == S2) (*it).p2 = S;
        if ((*it).p1 == (*it).p2) {
          // Erase by swapping with last element and popping the back
          (*it).edge = edge_functions.back().edge;
          (*it).first = edge_functions.back().first;
          (*it).second = edge_functions.back().second;
          (*it).p1 = edge_functions.back().p1;
          (*it).p2 = edge_functions.back().p2;
          edge_functions.pop_back();
        } else {
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

