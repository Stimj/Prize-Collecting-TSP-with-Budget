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

#include <string>

#include <problem.h>

// Reads data from TSPLIB or OPLIB files.
bool loadProblem(const std::string& filename, Problem& problem);
