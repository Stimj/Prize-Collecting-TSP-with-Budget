//
//  Subroutine.hpp
//  Prize-Collecting TSP
//
//  Created by Alice Paul on 3/14/17.
//
//  MIT License
//  Copyright (c) 2020 alicepaul
//

#pragma once

#include <stdio.h>
#include <climits>
#include <cmath>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#include "graph.h"
#include "subset.h"

/* ------------------------- HELPER FUNCTIONS --------------------------*/

/* ------------------------- MAIN FUNCTIONS --------------------------*/


// Prune Function
// Prune inactive components from s and add all non-pruned edges to edges
// If l_plus = true then use alternate edges and consider tied subsets as
// inactive

double prune(std::shared_ptr<Subset>& s,
             std::list<std::shared_ptr<Edge>>& edges, bool l_plus = false,
             bool swap = true);

// Modification of the prune function which finds a path of maximal pruned
// subsets that are not pruned when s is marked active. Path is saved to prunedS
// and prunedE

void maxPrunedS(std::shared_ptr<Subset>& s,
                std::list<std::shared_ptr<Edge>>& edges,
                std::shared_ptr<Subset>& tied_S,
                std::vector<std::shared_ptr<Subset>>& prunedS,
                std::vector<std::shared_ptr<Edge>>& prunedE);

// Modification of the prune function which finds a path of maximal pruned
// subsets that are not pruned when s uses alt edge. Path is saved to prunedS
// and prunedE

void maxPrunedE(std::shared_ptr<Subset>& s,
                std::list<std::shared_ptr<Edge>>& edges,
                std::shared_ptr<Subset>& tied_S,
                std::vector<std::shared_ptr<Subset>>& prunedS,
                std::vector<std::shared_ptr<Edge>>& prunedE);

// Reverse Delete Function
// For each component in subsets, call prune to find remaining tight edges
// Returns the largest sum of tight edge weights and updates edges to contain
// those edges

double reverseDelete(std::list<std::shared_ptr<Subset>>& subsets,
                     std::list<std::shared_ptr<Edge>>& edges,
                     std::shared_ptr<Subset>& largestSub, bool l_plus = false,
                     bool swap = true);

/* ------------------------- ALTERNATE FUNCTIONS --------------------------*/

// To not keep track of edges
double reverseDelete(std::list<std::shared_ptr<Subset>>& subsets,
                     bool l_plus = false, bool swap = true);
// Same as above but with a single component
double reverseDelete(std::shared_ptr<Subset>& s,
                     std::list<std::shared_ptr<Edge>>& edges,
                     bool l_plus = false, bool swap = true);
// Same as above with a single component but don't keep track of edges
double reverseDelete(std::shared_ptr<Subset>& s, bool l_plus = false,
                     bool swap = true);

