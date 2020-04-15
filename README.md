# Prize-Collecting-TSP-with-Budget

This repository is based on code for the algorithm described in the paper [Budgeted Prize-Collecting Traveling Salesman and Minimum Spanning Tree Problems.](https://pubsonline.informs.org/doi/abs/10.1287/moor.2019.1002)

It has some improvements to data structures which makes it ~40x faster. The core algorithm is the same as cited in the paper.

## Quickstart
* Install [bazel](https://docs.bazel.build/versions/master/install.html)
* Run the demo: `bazel build -c opt :demo` which will solve the bier127.tsp instance from TSPLIB
* Run the tests: `bazel test -c opt ...`
