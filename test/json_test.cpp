#include "graph.h"
#include "to_json.h"

#include "gtest/gtest.h"

#include <iostream>

TEST(JsonTest, edge) {
  nlohmann::json j;
  auto edge = Edge(0, 42, 3.14159);
  j["edge"] = edge;

  auto e_ptr = std::make_shared<Edge>(1, 43, 2.718281828);
  j["edge_ptr"] = e_ptr;

  std::cout << j.dump(4) << std::endl;
}

TEST(JsonTest, vertex) {
  nlohmann::json j;

  auto e_ptr = std::make_shared<Edge>(1, 43, 2.718281828);
  auto vertex = Vertex(0, 2.0);
  vertex.addEdge(e_ptr);
  j["vertex"] = vertex;

  std::cout << j.dump(4) << std::endl;

  EXPECT_TRUE(true);
}

TEST(JsonTest, subgraph) {
  nlohmann::json j;
  auto subset_0 = std::make_shared<Subset>(0, 1.0);  // vertices {0}
  auto subset_1 = std::make_shared<Subset>(1, 1.0);  // vertices {0}
  auto edge_01 =
      std::make_shared<Edge>(0, 1, 0.2);  // Edge from 0->1 with weight 0.2
  Subset subset_01(subset_0, subset_1, edge_01);
  j["subset_0"] = subset_0;
  j["subset_1"] = subset_1;
  j["edge_01"] = edge_01;
  j["subset_01"] = subset_01;

  std::cout << j.dump(4) << std::endl;
}
