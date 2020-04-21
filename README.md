# Prize-Collecting-TSP-with-Budget

This repository is based on code for the algorithm described in the paper [Budgeted Prize-Collecting Traveling Salesman and Minimum Spanning Tree Problems.](https://pubsonline.informs.org/doi/abs/10.1287/moor.2019.1002) The goal is to maximize the number of vertices in the returned tour/tree subject to a bound on the tour/tree cost. The optimal prize is guaranteed to be no more than 2 + epsilon times the prize found.

This repository optimizes some data structures to improve runtime. The algorithm itself is the same as in the original paper.

## Quickstart
* Install [bazel](https://docs.bazel.build/versions/master/install.html)
* Run the demo: `bazel build -c opt :demo` which will solve the bier127.tsp instance from TSPLIB
* Run the integration test: `bazel test -c opt :solutions_baseline_test` which will solve instances from the TSPLIB and compare to previous solutions
