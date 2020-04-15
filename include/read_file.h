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

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

#include "graph.h"
#include "pd.h"
#include "subroutine.h"
#include "subset.h"

void tokenize(const std::string &str, std::vector<std::string> &tokens,
              const std::string &delimiters);

bool has_suffix(const std::string &str, const std::string &suffix);

bool has_prefix(const std::string &str, const std::string &prefix);

bool is_number(const std::string &s);

int graphFromFile(const std::string &fileName, Graph &G, std::string &graphName,
                  double &meanEdgeWeight, int &numNodes);

std::map<std::pair<int, int>, double> readDistances(
    const std::string &fileName);

