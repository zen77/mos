#pragma once

#include <glm/glm.hpp>
#include <array>
#include <mos/render/camera.hpp>

namespace mos {
namespace gfx {
class RenderCubeCamera {
private:
  void update_views();
  glm::mat4 projection;
  glm::vec3 up;
public:
  RenderCubeCamera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 1.25f),
                   const float aspect_ratio = 1.0f);
  RenderCubeCamera(const RenderCubeCamera &camera);
  RenderCubeCamera &operator=(const RenderCubeCamera &other);

  glm::vec3 position() const;
  std::array<RenderCamera, 6> cameras;
};
}
}