#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include <graph.h>
#include <pd.h>
#include <read_file.h>

// Sort in order of nodes
const std::vector<std::string> kReadableInstances{
    "burma14.tsp", "ulysses16.tsp", "ulysses22.tsp", "att48.tsp",
    "eil51.tsp",   "berlin52.tsp",  "st70.tsp",      "eil76.tsp",
    "pr76.tsp",    "gr96.tsp",      "rat99.tsp",     "rd100.tsp",
    "kroA100.tsp", "kroB100.tsp",   "kroC100.tsp",   "kroD100.tsp",
    "kroE100.tsp", "eil101.tsp",    "lin105.tsp",    "pr107.tsp",
    "pr124.tsp",   "bier127.tsp",   "ch130.tsp",     "pr136.tsp",
    "gr137.tsp",   "pr144.tsp",     "ch150.tsp",     "kroA150.tsp",
    "kroB150.tsp", "pr152.tsp",     "u159.tsp",      "rat195.tsp",
    "d198.tsp",    "kroA200.tsp",   "kroB200.tsp",   "gr202.tsp",
    "ts225.tsp",   "tsp225.tsp",    "pr226.tsp",     "gr229.tsp",
    "gil262.tsp",  "pr264.tsp",     "a280.tsp",      "pr299.tsp",
    "lin318.tsp",  "rd400.tsp",     "fl417.tsp",     "gr431.tsp",
    "pr439.tsp",   "pcb442.tsp",    "d493.tsp",      "att532.tsp",
    "ali535.tsp",  "u574.tsp",      "rat575.tsp",    "p654.tsp",
    "d657.tsp",    "gr666.tsp",     "u724.tsp",      "rat783.tsp",
    "dsj1000.tsp", "pr1002.tsp",    "u2319.tsp"};

bool solveProblem(const std::string& filename, double max_solve_time,
                  std::ofstream& statistics_file) {
  SolverInfo info;
  if (!loadProblem("tsplib_benchmarks/" + filename, info.problem)) return true;
  info.problem.time_limit = max_solve_time;
  std::list<std::shared_ptr<Edge>> mst;
  info.problem.budget = 0.5 * info.problem.graph.MST(mst);

  solveInstance(info);

  // Record information to csv output file
  // num_nodes, solution_time, upper_bound, prize, solution_found, file_name
  statistics_file << info.problem.graph.getVertices().size() << ", "
                  << info.walltime << ", " << info.solution.upper_bound << ", "
                  << info.solution.prize << ", " << info.solution.solved << ", "
                  << filename << std::endl;

  std::cout << "Search finished after " << info.walltime << " seconds\n";
  std::cout << "Solution found? " << info.solution.solved
            << "\nUpper bound: " << info.solution.upper_bound
            << "\nPrize: " << info.solution.prize << std::endl;
  return info.walltime < max_solve_time;
}

int main(int argc, char* argv[]) {
  // TODO: Automatically put this in the right place
  std::ofstream statistics_file("solve_times.csv");

  statistics_file << "# num_nodes, solution_time, upper_bound, prize, "
                     "solution_found, file_name"
                  << std::endl;

  // Stop generating statistics if solutions take > 300 seconds.
  double max_solve_time = 300.0;

  for (auto file : kReadableInstances) {
    if (!solveProblem(file, max_solve_time, statistics_file)) break;
  };
  statistics_file.close();
}
