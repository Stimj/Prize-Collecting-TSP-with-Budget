//
//  PD.hpp
//  Prize-Collecting TSP
//
//  Created by Alice Paul on 3/20/17.
//
//  MIT License
//  Copyright (c) 2020 alicepaul
//

#pragma once

#include <chrono>
#include <climits>
#include <cmath>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <stdio.h>
#include <unordered_map>
#include <vector>

#include "graph.h"
#include "grow_subsets.h"
#include "problem.h"
#include "prune.h"
#include "subset.h"

/* ------------------------- HELPER FUNCTIONS--------------------------*/

// Wrapper which solves problem instance and stores relevant solution information
void solveInstance(SolverInfo& info);

// Change all edges to alt edges
void reverseEdges(std::shared_ptr<Subset> &s);

// Calculate prize of all vertices in tree
int prizeTree(const Graph &G, std::list<std::shared_ptr<Edge>> &tree);

// Finds initial l and r values such that PD(l+) > 0.5 D and PD(r-) <= 0.5 D
void findLR(const Graph &G, double D, double &l, double &r);

// Find all edges between subsets with alt edges and find all subsets marked
// tied
void findTies(const std::shared_ptr<Subset> &s,
              std::list<std::shared_ptr<Subset>> &tiedEdges,
              std::list<std::shared_ptr<Subset>> &tiedSubsets,
              bool swap = true);

/* ------------------------- MAIN FUNCTIONS--------------------------*/

double findLambdaBin(const Graph &G, double D);

// Use binary search to find theshold value lambda such that PD(lambda-) > 0.5*D
// and PD(lambda+) <= 0.5D
double findLambdaBin(const Graph &G, double D, bool &found, bool &swap,
                     bool &reversed, double max_solve_time);

// Find tree within 0.5*D and save to edges
// Tree is formed by pruning edges in reverseDelete(s) which starts > 0.5*D
std::shared_ptr<Edge> findTree(std::shared_ptr<Subset> &s, double D,
                               std::list<std::shared_ptr<Edge>> &edges,
                               bool swap = true);

// Main function
// Runs the overall primal dual algorithm on G to find a tree of weight <= 0.5*D
// saved to edges An upper bound on opt is saved to upper and the number of
// recursions in recursions (start with zero) Recurse = true or false whether or
// not you recurse The function returns the number of visited vertices
int PD(const Graph &G, double D, std::list<std::shared_ptr<Edge>> &edges,
       double &upper, int &recursions, double &lambda, bool &found,
       bool recurse = true, double max_solve_time = INT_MAX);
