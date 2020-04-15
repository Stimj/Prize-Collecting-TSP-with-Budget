#pragma once
#include <unordered_map>
#include "problem.h"
const std::unordered_map<std::string, SolverInfo> kBaselineDatabase = {
{"a280.tsp", SolverInfo{{{}, 1219.28}, {1, {}, 77, 148.519}, 0., 0, 0.}},
{"att48.tsp", SolverInfo{{{}, 13821.8}, {1, {}, 19, 31.7475}, 0., 0, 0.}},
{"berlin52.tsp", SolverInfo{{{}, 857.5}, {1, {}, 30, 48.8172}, 0., 0, 0.}},
{"berlin52.tsp", SolverInfo{{{}, 857.5}, {1, {}, 30, 48.8172}, 0., 0, 0.}},
{"bier127.tsp", SolverInfo{{{}, 47358.8}, {1, {}, 66, 111.797}, 0., 0, 0.}},
{"burma14.tsp", SolverInfo{{{}, 5.5}, {1, {}, 6, 11.3384}, 0., 0, 0.}},
{"ch130.tsp", SolverInfo{{{}, 346}, {1, {}, 42, 79.081}, 0., 0, 0.}},
{"ch150.tsp", SolverInfo{{{}, 343.5}, {1, {}, 43, 81.2838}, 0., 0, 0.}},
{"d198.tsp", SolverInfo{{{}, 4.5}, {1, {}, 183, 189.017}, 0., 0, 0.}},
{"d493.tsp", SolverInfo{{{}, 1.5}, {1, {}, 243, 493}, 0., 0, 0.}},
{"eil101.tsp", SolverInfo{{{}, 281.129}, {1, {}, 37, 64.9749}, 0., 0, 0.}},
{"eil51.tsp", SolverInfo{{{}, 188.245}, {1, {}, 15, 29.2042}, 0., 0, 0.}},
{"eil76.tsp", SolverInfo{{{}, 236.165}, {1, {}, 23, 44.1448}, 0., 0, 0.}},
{"fl417.tsp", SolverInfo{{{}, 4}, {1, {}, 243, 337.939}, 0., 0, 0.}},
{"gil262.tsp", SolverInfo{{{}, 1051.55}, {1, {}, 86, 157.1}, 0., 0, 0.}},
{"gr137.tsp", SolverInfo{{{}, 62}, {1, {}, 61, 112.384}, 0., 0, 0.}},
{"gr202.tsp", SolverInfo{{{}, 20.5}, {1, {}, 129, 198.825}, 0., 0, 0.}},
{"gr229.tsp", SolverInfo{{{}, 57}, {1, {}, 123, 191.977}, 0., 0, 0.}},
{"gr431.tsp", SolverInfo{{{}, 60.5}, {1, {}, 302, 428.361}, 0., 0, 0.}},
{"gr96.tsp", SolverInfo{{{}, 36.5}, {1, {}, 43, 67.8826}, 0., 0, 0.}},
{"kroA100.tsp", SolverInfo{{{}, 9386.09}, {1, {}, 31, 56.2635}, 0., 0, 0.}},
{"kroA150.tsp", SolverInfo{{{}, 11778.7}, {1, {}, 47, 89.3963}, 0., 0, 0.}},
{"kroA200.tsp", SolverInfo{{{}, 12966.3}, {1, {}, 61, 118.468}, 0., 0, 0.}},
{"kroB100.tsp", SolverInfo{{{}, 9628.48}, {1, {}, 32, 56.4433}, 0., 0, 0.}},
{"kroB150.tsp", SolverInfo{{{}, 11402.1}, {1, {}, 46, 85.3655}, 0., 0, 0.}},
{"kroB200.tsp", SolverInfo{{{}, 13101.8}, {1, {}, 62, 116.28}, 0., 0, 0.}},
{"kroC100.tsp", SolverInfo{{{}, 9201.16}, {1, {}, 27, 55.5275}, 0., 0, 0.}},
{"kroD100.tsp", SolverInfo{{{}, 9298.36}, {1, {}, 32, 60.6499}, 0., 0, 0.}},
{"kroE100.tsp", SolverInfo{{{}, 9612.2}, {1, {}, 35, 63.3361}, 0., 0, 0.}},
{"lin105.tsp", SolverInfo{{{}, 6530.22}, {1, {}, 38, 73.7717}, 0., 0, 0.}},
{"lin318.tsp", SolverInfo{{{}, 18959.4}, {1, {}, 104, 205.153}, 0., 0, 0.}},
{"pcb442.tsp", SolverInfo{{{}, 4.5}, {1, {}, 354, 408.15}, 0., 0, 0.}},
{"pr107.tsp", SolverInfo{{{}, 17378.7}, {1, {}, 34, 64.3747}, 0., 0, 0.}},
{"pr124.tsp", SolverInfo{{{}, 25267.7}, {1, {}, 47, 78.3947}, 0., 0, 0.}},
{"pr136.tsp", SolverInfo{{{}, 44483.1}, {1, {}, 38, 72.6755}, 0., 0, 0.}},
{"pr144.tsp", SolverInfo{{{}, 24732.2}, {1, {}, 40, 81.8445}, 0., 0, 0.}},
{"pr152.tsp", SolverInfo{{{}, 29584.2}, {1, {}, 49, 101.012}, 0., 0, 0.}},
{"pr226.tsp", SolverInfo{{{}, 34320}, {1, {}, 86, 165.355}, 0., 0, 0.}},
{"pr264.tsp", SolverInfo{{{}, 20571.1}, {1, {}, 88, 168.644}, 0., 0, 0.}},
{"pr299.tsp", SolverInfo{{{}, 21246.6}, {1, {}, 87, 170.21}, 0., 0, 0.}},
{"pr439.tsp", SolverInfo{{{}, 46097}, {1, {}, 163, 323.752}, 0., 0, 0.}},
{"pr76.tsp", SolverInfo{{{}, 43608.9}, {1, {}, 25, 49.037}, 0., 0, 0.}},
{"rat195.tsp", SolverInfo{{{}, 1082.4}, {1, {}, 51, 100.751}, 0., 0, 0.}},
{"rat99.tsp", SolverInfo{{{}, 557.365}, {1, {}, 28, 53.7956}, 0., 0, 0.}},
{"rd100.tsp", SolverInfo{{{}, 4}, {1, {}, 36, 59.1573}, 0., 0, 0.}},
{"rd400.tsp", SolverInfo{{{}, 4}, {1, {}, 147, 234.967}, 0., 0, 0.}},
{"st70.tsp", SolverInfo{{{}, 283.103}, {1, {}, 22, 41.6016}, 0., 0, 0.}},
{"ts225.tsp", SolverInfo{{{}, 56000}, {1, {}, 57, 112.92}, 0., 0, 0.}},
{"tsp225.tsp", SolverInfo{{{}, 234.5}, {1, {}, 87, 140.278}, 0., 0, 0.}},
{"u159.tsp", SolverInfo{{{}, 2.5}, {1, {}, 59, 102.465}, 0., 0, 0.}},
{"ulysses16.tsp", SolverInfo{{{}, 4}, {1, {}, 9, 12.0075}, 0., 0, 0.}},
{"ulysses22.tsp", SolverInfo{{{}, 4}, {1, {}, 12, 17.0099}, 0., 0, 0.}},
};
