//
//  PD.cpp
//  Prize-Collecting TSP
//
//  Created by Alice Paul on 3/21/17.
//
//  MIT License
//  Copyright (c) 2020 alicepaul
//

#include "pd.h"

double ep = 1.0e-15;  // theshold for ties

/* ------------------------- HELPER FUNCTIONS--------------------------*/

void solveInstance(SolverInfo &info) {
  auto t0 = std::chrono::high_resolution_clock::now();
  PD(info.problem.graph, info.problem.budget, info.solution.path,
     info.solution.upper_bound, info.recursions, info.lambda,
     info.solution.solved, true, info.problem.time_limit);
  auto t1 = std::chrono::high_resolution_clock::now();
  info.solution.prize = prizeTree(info.problem.graph, info.solution.path);
  info.walltime =
      static_cast<double>(
          std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)
              .count()) /
      1000000.;
}

// Change all edges to alt edges
void reverseEdges(std::shared_ptr<Subset> &s) {
  // Base case is that s does not have a parent
  if (s->getParent1() == NULL) {
    return;
  }

  // Otherwise check if alternative edge is different from min edge
  if ((s->getAltEdge() != NULL) && (s->getAltEdge() != s->getEdge())) {
    s->swapEdges();
  }

  // Recurse on parents
  std::shared_ptr<Subset> p1 = s->getParent1(), p2 = s->getParent2();
  reverseEdges(p1);
  reverseEdges(p2);
}

// Calculate prize of all vertices in tree
int prizeTree(const Graph &G, std::list<std::shared_ptr<Edge>> &tree) {
  int p = 0;
  std::unordered_map<int, bool> added;
  for (auto v : G.getVertices()) {
    added[v] = false;
  }

  for (auto e : tree) {
    int i = e->getHead(), j = e->getTail();
    if (added[i] == false) {
      p += G.getVertex(i)->getPrize();
      added[i] = true;
    }
    if (added[j] == false) {
      p += G.getVertex(j)->getPrize();
      added[j] = true;
    }
  }

  return p;
}

// Finds initial l and r values such that PD(l+) > 0.5 D and PD(r-) <= 0.5 D
void findLR(const Graph &G, double D, double &l, double &r) {
  // Find min and max non-zero edge weights
  double min_w = INT_MAX, max_w = -INT_MAX;
  for (auto e : G.getEdges()) {
    if ((e->getWeight() > ep) && (e->getWeight() < min_w)) {
      min_w = e->getWeight();
    }
    if (e->getWeight() > max_w) {
      max_w = e->getWeight();
    }
  }

  // Set initial l and r
  l = 1 / (2 * max_w) - ep;
  r = G.getPrize() / (min_w) + 1;

  // Check that l and r satisfy properties
  GrowSubsets g;
  std::list<std::shared_ptr<Subset>> subsetsL = g.build(G, l);
  double weight_l = reverseDelete(subsetsL, true);
  if (weight_l <= 0.5 * D) {
    std::cout << l << " , " << weight_l << "\n";
    throw std::invalid_argument("Left point not satisfied");
  }

  GrowSubsets g2;
  std::list<std::shared_ptr<Subset>> subsetsR = g2.build(G, r);
  double weight_r = reverseDelete(subsetsR, false);
  if (weight_r >= 0.5 * D) {
    std::cout << r << " , " << weight_r << "\n";
    throw std::invalid_argument("Right point not satisfied");
  }
}

// Find all tight edges between subsets with alt edges and all subsets marked
// tied
void findTies(const std::shared_ptr<Subset> &s,
              std::list<std::shared_ptr<Subset>> &tiedEdges,
              std::list<std::shared_ptr<Subset>> &tiedSubsets, bool swap) {
  // If tied then push back
  if (s->getTied() && s->getActive()) {
    tiedSubsets.push_back(s);
  }

  // Base case is that s does not have a parent
  if (s->getParent1() == NULL) {
    return;
  }

  // Otherwise check if alternative edge is different from min edge
  if ((s->getAltEdge() != NULL) && (s->getAltEdge() != s->getEdge()) &&
      (swap)) {
    tiedEdges.push_back(s);
  }

  // Recurse on parents
  findTies(s->getParent1(), tiedEdges, tiedSubsets, swap);
  findTies(s->getParent2(), tiedEdges, tiedSubsets, swap);
}

/* ------------------------- MAIN FUNCTIONS--------------------------*/

double findLambdaBin(const Graph &G, double D) {
  bool found, swap, reversed;
  return findLambdaBin(G, D, found, swap, reversed, static_cast<double>(INT_MAX));
}

// Use binary search to find theshold value lambda such that PD(lambda-) > 0.5*D
// and PD(lambda+) <= 0.5D
double findLambdaBin(const Graph &G, double D, bool &found, bool &swap,
                     bool &reversed, double max_solve_time) {
  auto t0 = std::chrono::high_resolution_clock::now();
  // Find initial l and r
  double l, r;
  findLR(G, D, l, r);
  int iters = 0;
  double diff = ep;
  swap = true, reversed = false;

  // Do binary search
  while (l * (1 + diff) <= r) {
    auto t1 = std::chrono::high_resolution_clock::now();
    if (static_cast<double>(
            std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count())/1000000. >
        max_solve_time) {
      found = false;
      return -1;
    }
    double p = (l + r) / 2;
    // std::cout << "iters: " << iters << " l: " << l << " r: " << r << " p: "
    // << p << "\n";
    iters += 1;
    GrowSubsets g;
    std::list<std::shared_ptr<Subset>> subsets = g.build(G, p);
    double wminus = reverseDelete(subsets, false),
           wplus = reverseDelete(subsets, true, true);
    double wplusalt = reverseDelete(subsets, true, false);  // don't swap edges
    // std::cout << wminus << "," << wplus << "," << wplusalt << "\n";
    if ((wminus > 0.5 * D) && (wplus <= 0.5 * D)) {
      found = true;
      return p;
    } else if ((wminus > 0.5 * D) && (wplusalt <= 0.5 * D)) {
      swap = false;
      found = true;
      return p;
    } else if ((wminus <= 0.5 * D) && (wplus > 0.5 * D)) {
      reversed = true;
      found = true;
      return p;
    } else if (wminus <= 0.5 * D) {
      r = p;
    } else {
      l = p;
    }
  }

  found = false;
  return r;
}

// Find tree by pruning reverseDelete(s,true) > 0.5*D, reverseDelete(s,false) <=
// 0.5*D
std::shared_ptr<Edge> findTree(std::shared_ptr<Subset> &s, double D,
                               std::list<std::shared_ptr<Edge>> &edges,
                               bool swap) {
  // First find list of alternative edges and subsets
  std::list<std::shared_ptr<Subset>> tiedEdges;
  std::list<std::shared_ptr<Subset>> tiedSubsets;
  findTies(s, tiedEdges, tiedSubsets, swap);
  // std::cout << "Currently above weight " << reverseDelete(s);

  // Find path of pruned subsets
  std::vector<std::shared_ptr<Subset>> subsetsPruned;
  std::vector<std::shared_ptr<Edge>> edgesPruned;

  // First go through tiedEdges and change to alternate edges
  bool broke = false;
  for (auto test : tiedEdges) {
    // std::cout << "Test " << *test << "\n";
    // See if change brings above threshold
    std::shared_ptr<Edge> e = test->getEdge(), alt_e = test->getAltEdge();
    test->swapEdges();
    if (reverseDelete(s) <= 0.5 * D) {
      std::list<std::shared_ptr<Edge>> temp_e;
      maxPrunedE(s, temp_e, test, subsetsPruned, edgesPruned);
      broke = true;
      break;
    }
  }

  // Next go through tiedSubsets and change to inactive
  if (broke != true) {
    for (auto test : tiedSubsets) {
      // std::cout << "Test " << *test;
      // See if change brings above threshold
      test->setActive(false);
      if (reverseDelete(s) <= 0.5 * D) {
        // std::cout << "Broke here \n";
        std::list<std::shared_ptr<Edge>> temp_e;
        maxPrunedS(s, temp_e, test, subsetsPruned, edgesPruned);
        broke = true;
        break;
      }
    }
  }

  // Find starting edges and visited vertices
  std::list<std::shared_ptr<Edge>> tree;
  std::shared_ptr<Edge> break_e;
  double w = prune(s, tree);

  // std::cout << "Starting at weight " << w << "\n";

  std::set<int> visited;
  for (auto e : tree) {
    visited.insert(e->getHead()), visited.insert(e->getTail());
  }

  // While not above the limit find the next pruned subset to pick from
  for (size_t i = 0; i < subsetsPruned.size(); i++) {
    // std::cout << "Current weight " << w << "\n";
    // std::cout << "Pruned " << *subsetsPruned[i];
    // std::cout << "With edge " << *edgesPruned[i] << "\n";
    std::shared_ptr<Edge> test_e = edgesPruned[i];
    double w_e = test_e->getWeight();
    // If getting to next subset brings over limit then end
    if (w + w_e > 0.5 * D) {
      break_e = test_e;
      break;
    }

    // Otherwise add e and go to next subset
    else {
      tree.push_back(test_e);
      w += w_e;
      // Find endpoint in e not visited yet (labeled v)
      int u = test_e->getHead(), v = test_e->getTail();
      if (visited.count(u) == 0) {
        u = test_e->getTail(), v = test_e->getHead();
      }
      visited.insert(v);

      // Pick from subset
      std::shared_ptr<Edge> last_e = NULL;
      double added =
          pick(subsetsPruned[i], visited, 0.5 * D - w, v, tree, last_e);
      w += added;

      // If didn't add all edges in the subset then stop
      if (last_e != NULL) {
        break_e = last_e;
        break;
      }
    }
  }

  // Set edges to tree
  edges = tree;
  return break_e;
}

// Main function
int PD(const Graph &G, double D, std::list<std::shared_ptr<Edge>> &edges,
       double &upper, int &recursions, double &lambda, bool &found,
       bool recurse, double max_solve_time) {
  auto t0 = std::chrono::high_resolution_clock::now();
  recursions = 1;

  std::cout << " -------- Starting Alg ----------- \n";
  // std::cout << " GRAPH: " << G;

  // If a MST is feasible, return
  std::list<std::shared_ptr<Edge>> mst;
  double mst_w = G.MST(mst);
  std::cout << "- MST weight: " << mst_w << "\n";
  if (mst_w <= 0.5 * D) {
    // std::cout << "Returning MST of weight " << mst_w << "\n";
    edges = mst;
    upper = G.getPrize();
    return G.getPrize();
  }

  std::cout << "- Total Prize " << G.getPrize() << "\n";

  std::cout << " --------------------------------- \n";
  // Otherwise find threshold lambda
  bool swap = true, reversed = false;
  lambda = findLambdaBin(G, D, found, swap, reversed, max_solve_time);
  if (!found) {
    upper = 0.0;
    edges.clear();
    return 0;
  }
  std::cout << "- Lambda1: " << lambda << "\n";
  // std::cout << "- Found: " << found << "\n";

  // Then find largest subsets
  GrowSubsets g;
  std::list<std::shared_ptr<Subset>> subsets = g.build(G, lambda);
  // If reversed (wplus > 0.5*D) then we need to start with reversed edges
  if (reversed) {
    for (auto t : subsets) {
      reverseEdges(t);
    }
  }
  std::list<std::shared_ptr<Edge>> tempE;
  std::shared_ptr<Subset> s = NULL;
  reverseDelete(subsets, tempE, s, false);

  // Find associated pruned tree
  std::list<std::shared_ptr<Edge>> tree;
  std::shared_ptr<Edge> last_e = findTree(s, D, tree, swap);
  int currPrize = prizeTree(G, tree);
  std::cout << "- Prize of tree found: " << currPrize << "\n";

  // Compute weight
  double wTree = 0;
  for (auto e : tree) {
    wTree += e->getWeight();
  }
  std::cout << "- Weight of tree: " << wTree << "\n";
  // if (last_e != NULL)
  //    std::cout << "- Weight of last edge: " << last_e->getWeight() << "\n";

  // Calculate upper bound by finding subset with highest potential
  std::shared_ptr<Subset> max_s = findMaxPotential(subsets, currPrize);
  upper = lambda * D + max_s->getPotential();
  if (upper > G.getPrize()) {
    upper = G.getPrize();
  }
  std::cout << "- Upper bound: " << upper << "\n";

  // Find set with highest potential that contains tree
  double p = max_s->getPotential() - 0.0001;
  if (tree.size() > 1) {
    std::list<std::shared_ptr<Edge>> treebar = tree;
    if (last_e != NULL) {
      treebar.push_back(last_e);
    }
    std::shared_ptr<Subset> W = findMaxSuperset(s, treebar);
    p = W->getPotential();
  }
  // std::cout << "- Potential of W: " << p << "\n";

  // Reduce max_solve_time by amount of time this routine took

  auto t1 = std::chrono::high_resolution_clock::now();
  max_solve_time -= static_cast<double>(
      std::chrono::duration_cast<std::chrono::seconds>(t1 - t0).count());

  // Recurse on subgraphs with high potential and return best found
  if (recurse) {
    std::list<std::shared_ptr<Subset>> altS = findHighPotential(subsets, p);
    std::cout << "-------- Recursing " << altS.size() << " times -------- \n";
    for (auto test_s : altS) {
      if (test_s->getPrize() > currPrize) {
        Graph H(G, test_s->getVertices());  // Find subgraph
        double test_upper;
        int test_recursions;
        double test_lambda;
        bool test_found;
        std::list<std::shared_ptr<Edge>> test_e;
        PD(H, D, test_e, test_upper, test_recursions, test_lambda, test_found,
           recurse, max_solve_time);  // Recurse
        recursions += test_recursions;

        // If better than current tree then replace
        int test_p = prizeTree(G, test_e);
        if (test_p > currPrize) {
          tree = test_e;
          currPrize = test_p;
        }
      }
    }
  }

  // std::cout << "---------- Done Recursing -------------- \n";
  // std::cout << "- New best: " << currPrize << "\n";
  edges = tree;

  return currPrize;
}
