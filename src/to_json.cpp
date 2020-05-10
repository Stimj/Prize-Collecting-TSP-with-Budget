#include "to_json.h"

void to_json(nlohmann::json& j, const Edge& e) {
  j = nlohmann::json{
      {"head", e.getHead()}, {"tail", e.getTail()}, {"weight", e.getWeight()}};
}

void to_json(nlohmann::json& j, const std::shared_ptr<Edge>& e) {
  if (e == nullptr) {
    j = nlohmann::json{};
  } else {
    to_json(j, *e);
  }
}

void to_json(nlohmann::json& j, const Vertex& v) {
  j = nlohmann::json{{"id", v.getId()},
                     {"prize", v.getPrize()},
                     {"neighbors", v.getIncEdges()}};
}

void to_json(nlohmann::json& j, const std::shared_ptr<Vertex>& v) {
  if (v == nullptr) {
    j = nlohmann::json{};
  } else {
    to_json(j, *v);
  }
}

void to_json(nlohmann::json& j, const Subset& s) {
  j = nlohmann::json{
      {"vertices", s.getVertices()},   {"active", s.getActive()},
      {"potential", s.getPotential()}, {"y_val", s.getY()},
      {"tied", s.getTied()},           {"prize", s.getPrize()},
      {"edge", s.getEdge()},           {"alt_edge", s.getAltEdge()}};
}

void to_json(nlohmann::json& j, const std::shared_ptr<Subset>& s) {
  if (s == nullptr) {
    j = nlohmann::json{};
  } else {
    to_json(j, *s);
  }
}
