#pragma once
#include <glm/glm.hpp>

namespace mos {
namespace sim {

/** Intersection from a ray test. */
class Intersection {
public:
  Intersection(const glm::vec3 &position, const glm::vec3 &normal);
  ~Intersection();
  glm::vec3 position;
  glm::vec3 normal;
};
}
}