#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "graph.h"
#include "pd.h"
#include "read_file.h"

const std::vector<std::string> kReadableInstances{
    "a280.tsp",     "ali535.tsp",   "att48.tsp",     "att532.tsp",
    "berlin52.tsp", "berlin52.tsp", "bier127.tsp",   "burma14.tsp",
    "ch130.tsp",    "ch150.tsp",    "d198.tsp",      "d493.tsp",
    "d657.tsp",     "dsj1000.tsp",  "eil101.tsp",    "eil51.tsp",
    "eil76.tsp",    "fl417.tsp",    "gil262.tsp",    "gr137.tsp",
    "gr202.tsp",    "gr229.tsp",    "gr431.tsp",     "gr666.tsp",
    "gr96.tsp",     "kroA100.tsp",  "kroA150.tsp",   "kroA200.tsp",
    "kroB100.tsp",  "kroB150.tsp",  "kroB200.tsp",   "kroC100.tsp",
    "kroD100.tsp",  "kroE100.tsp",  "lin105.tsp",    "lin318.tsp",
    "p654.tsp",     "pcb442.tsp",   "pr1002.tsp",    "pr107.tsp",
    "pr124.tsp",    "pr136.tsp",    "pr144.tsp",     "pr152.tsp",
    "pr226.tsp",    "pr264.tsp",    "pr299.tsp",     "pr439.tsp",
    "pr76.tsp",     "rat195.tsp",   "rat575.tsp",    "rat783.tsp",
    "rat99.tsp",    "rd100.tsp",    "rd400.tsp",     "st70.tsp",
    "ts225.tsp",    "tsp225.tsp",   "u159.tsp",      "u2319.tsp",
    "u574.tsp",     "u724.tsp",     "ulysses16.tsp", "ulysses22.tsp"};

void generateBaseline(const std::string& filename, int max_nodes,
                      std::ofstream& baseline_database) {
  SolverInfo info;

  double mean_edge_weight;
  int num_nodes;
  // read the graph from the file and get some stats from it
  auto success = graphFromFile("tsplib_benchmarks/" + filename,
                               info.problem.graph, mean_edge_weight, num_nodes);
  if (!success || num_nodes > max_nodes) return;

  std::list<std::shared_ptr<Edge>> mst;
  info.problem.budget = 0.5 * info.problem.graph.MST(mst);

  solveInstance(info);
  // Log information to a file

  // Print relevant solution information to header. Example:
  //{"bier127.tsp", SolverInfo{{{}, 47358.8}, {1, {}, 66, 111.797}, 0., 0, 0.}},
  baseline_database << "{\"" << filename << "\", SolverInfo{{{}, "
                    << info.problem.budget << "}, "
                    << "{" << info.solution.solved << ", {}, "
                    << info.solution.prize << ", " << info.solution.upper_bound
                    << "}, 0., 0, 0.}},\n";

  std::cout << "Search finished after " << info.walltime << " seconds\n";
  std::cout << "Solution found? " << info.solution.solved
            << "\nUpper bound: " << info.solution.upper_bound
            << "\nPrize: " << info.solution.prize << std::endl;
}

int main(int argc, char* argv[]) {
  // Open the file for writing
  // TODO: Automatically put this in the right place
  std::ofstream baseline_database("baseline_database.h");
  baseline_database
      << "#pragma once\n#include <unordered_map>\n#include \"problem.h\"\n"
      << "const std::unordered_map<std::string, SolverInfo> kBaselineDatabase "
         "= {\n";

  // Don't generate baseline for large problems
  int max_nodes = 800;

  for (auto file : kReadableInstances) {
    generateBaseline(file, max_nodes, baseline_database);
  };
  baseline_database << "};";
  baseline_database.close();
}
