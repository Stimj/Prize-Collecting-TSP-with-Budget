//
//  ReadFile.hpp
//
//
//  Created by Alice Paul on 4/10/17.
//
//  MIT License
//  Copyright (c) 2020 alicepaul
//

#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "graph.h"

// Reads TSPLIB instance from the given file.
// Returns true if parsing was successful.
bool graphFromFile(const std::string &filename, Graph &graph, double &mean_edge_weight, int &num_nodes);
