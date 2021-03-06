//
//  ReadFile.cpp
//
//
//  Created by Alice Paul on 4/10/17.
//
//

#include <stdio.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <read_file.h>
#include <graph.h>

std::vector<std::string> tokenize(const std::string &str,
                                  const std::string &delimiters) {
  std::vector<std::string> tokens;

  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos) {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
  return tokens;
}

struct Header {
  std::string name;
  std::string comment;
  std::string type;
  std::string edge_weight_type;
  std::string display_data_type;
  std::string data_section;
  int dimension;
  int cost_limit;
};

struct NodeCoord {
  int id;
  int x;
  int y;
};

Header readHeader(const std::string &filename) {
  std::ifstream input_file(filename);
  Header header;
  std::string line;
  while (header.type.empty() || header.data_section.empty()) {
    getline(input_file, line);
    if (line.empty()) {
      std::cout << "Reached end of file while parsing header\n";
      return header;
    }

    if (line.rfind("NODE_COORD_SECTION", 0) == 0 ||
        line.rfind("EDGE_WEIGHT_SECTION", 0) == 0) {
      header.data_section = line;
      continue;
    }

    auto words = tokenize(line, ": ");

    if (words.empty()) {
      std::cout << "Error parsing line:\n" << line << std::endl;
      continue;
    }

    if (words[0].rfind("NAME", 0) == 0) {
      header.name = words[1];
    } else if (words[0].rfind("TYPE", 0) == 0) {
      header.type = words[1];
    } else if (words[0].rfind("COMMENT", 0) == 0) {
      header.comment = words[1];  // should be words 1-end with spaces?
    } else if (words[0].rfind("DIMENSION", 0) == 0) {
      header.dimension = std::atoi(words[1].c_str());
    } else if (words[0].rfind("EDGE_WEIGHT_TYPE", 0) == 0) {
      header.edge_weight_type = words[1];
    } else if (words[0].rfind("DISPLAY_DATA_TYPE", 0) == 0) {
      header.display_data_type = words[1];
    } else if (words[0].rfind("COST_LIMIT", 0) == 0) {
      header.cost_limit = std::atoi(words[1].c_str());
    } else {
      std::cout << "Warning: Ignored token in preamble:\n" << line << std::endl;
    }
  }
  return header;
}

void printHeader(const Header &header) {
  std::cout << "\nNAME: " << header.name << "\nCOMMENT: " << header.comment
            << "\nTYPE: " << header.type
            << "\nEDGE_WEIGHT_TYPE: " << header.edge_weight_type
            << "\nDISPLAY_DATA_TYPE: " << header.display_data_type
            << "\nDATA_SECTION: " << header.data_section
            << "\nDIMENSION: " << header.dimension << std::endl
            << std::endl;
}

std::vector<int> readDepotSection(const std::string &filename) {
  std::ifstream input_file(filename);

  std::string line;
  std::vector<int> depots;

  // Skip to NODE_COORD_SECTION
  while (std::getline(input_file, line)) {
    if (line.empty()) {
      std::cout << "Error: DEPOT_SECTION not found\n";
      return {};
    }
    if (line.compare("DEPOT_SECTION") == 0) break;
  }
  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    int node_id;
    iss >> node_id;
    if (node_id != -1)
      depots.emplace_back(node_id);
    else
      break;
  }
  return depots;
}

double readNodeCoordSection(const std::string &filename, Graph &graph) {
  std::ifstream input_file(filename);

  std::string line;
  std::vector<NodeCoord> node_coordinates;

  // Skip to NODE_COORD_SECTION
  while (std::getline(input_file, line)) {
    if (line.empty()) {
      std::cout << "Error: NODE_COORD_SECTION not found\n";
      return 0.0;
    }
    if (line.compare("NODE_COORD_SECTION") == 0) break;
  }

  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    int node_id, x, y;
    iss >> node_id >> x >> y;
    node_coordinates.emplace_back(NodeCoord{node_id, x, y});
  }

  // Build the complete graph from the list of node coordinates
  double total_edge_weight = 0;
  int edges_added = 0;
  for (size_t head = 0; head < node_coordinates.size(); head++) {
    for (size_t tail = 0; tail < head; tail++) {
      // Compute distance between two points
      auto dx = node_coordinates[head].x - node_coordinates[tail].x;
      auto dy = node_coordinates[head].y - node_coordinates[tail].y;
      auto distance = std::sqrt(dx * dx + dy * dy);
      graph.addEdge(head, tail, distance);
      total_edge_weight += distance;
      edges_added++;
    }
    graph.addEdge(head, head, 0.0);
    edges_added++;
  }

  return total_edge_weight / edges_added;
}

int readNodeScoreSection(const std::string &filename, Graph &graph) {
  std::ifstream input_file(filename);

  std::string line;
  auto num_nodes = 0;

  // Skip to NODE_SCORE_SECTION
  while (std::getline(input_file, line)) {
    if (line.empty()) {
      std::cout << "Error: NODE_SCORE_SECTION not found\n";
      return 0;
    }
    if (line.compare("NODE_SCORE_SECTION") == 0) break;
  }

  while (std::getline(input_file, line)) {
    if (line.compare("DEPOT_SECTION") == 0) break;
    std::istringstream iss(line);
    int node_id, score;
    iss >> node_id >> score;
    graph.addVertex(node_id, score);
    num_nodes++;
  }
  return num_nodes;
}

bool graphFromFile(const std::string &filename, Graph &graph,
                   double &mean_edge_weight, int &num_nodes, int &cost_limit,
                   std::vector<int> &depots) {
  cost_limit = -1;
  // Test that file exists
  {
    std::ifstream infile(filename);
    if (!infile.good()) {
      std::cout << "Error opening file " << filename << std::endl;
      num_nodes = 0;
      return false;
    }
  }

  auto header = readHeader(filename);
  printHeader(header);

  // Load in data here
  num_nodes = 0;
  if (header.type.rfind("TSP", 0) == 0) {
    if (header.data_section.rfind("NODE_COORD_SECTION", 0) == 0) {
      for (int i = 0; i < header.dimension; ++i) {
        graph.addVertex(i);
        num_nodes++;
      }
      mean_edge_weight = readNodeCoordSection(filename, graph);
      if (mean_edge_weight <= 0.0) {
        std::cout << " Invalid mean edge weight " << mean_edge_weight
                  << " <= 0\n";
        num_nodes = 0;
        return false;
      }
    } else {
      std::cout << "Unsupported data section: " << header.data_section
                << std::endl;
      num_nodes = 0;
      return false;
    }
  } else if (header.type.rfind("OP", 0) == 0) {
    cost_limit = header.cost_limit;
    if (header.data_section.rfind("NODE_COORD_SECTION", 0) == 0) {
      num_nodes = readNodeScoreSection(filename, graph);
      if (num_nodes == 0) {
        std::cout << "Error reading node scores\n";
        return false;
      }
      mean_edge_weight = readNodeCoordSection(filename, graph);
      if (mean_edge_weight <= 0.0) {
        std::cout << " Invalid mean edge weight " << mean_edge_weight
                  << " <= 0\n";
        num_nodes = 0;
        return false;
      }
      depots = readDepotSection(filename);
    } else {
      std::cout << "Unsupported data section: " << header.data_section
                << std::endl;
      num_nodes = 0;
      return false;
    }
  } else {
    std::cout << "Unsupported filetype " << header.type << std::endl;
    num_nodes = 0;
    return false;
  }
  return true;
}

bool loadProblem(const std::string &filename, Problem &problem) {
  double mean_edge_weight = 0;
  int num_nodes = 0, budget;
  auto success = graphFromFile(filename, problem.graph, mean_edge_weight,
                               num_nodes, budget, problem.roots);
  problem.budget = static_cast<double>(budget);
  return success;
}
