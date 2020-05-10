#pragma once

#include "external/json.hpp"

#include "graph.h"
#include "subset.h"

/// @brief conversion functions for saving problem data as json

void to_json(nlohmann::json& j, const Edge& e);
void to_json(nlohmann::json& j, const std::shared_ptr<Edge>& e);

void to_json(nlohmann::json& j, const Vertex& v);
void to_json(nlohmann::json& j, const std::shared_ptr<Vertex>& v);

void to_json(nlohmann::json& j, const Subset& s);
void to_json(nlohmann::json& j, const std::shared_ptr<Subset>& s);

