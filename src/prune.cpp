//
//  Subroutine.cpp
//  Prize-Collecting TSP
//
//  Created by Alice Paul on 3/14/17.
//
//  MIT License
//  Copyright (c) 2020 alicepaul
//

#include "prune.h"

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

