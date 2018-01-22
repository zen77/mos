#include <mos/render/environment_light.hpp>

namespace mos {

EnvironmentLight::EnvironmentLight(const SharedTextureCube &texture,
                                   const glm::vec3 &position,
                                   const glm::vec3 &extent,
                                   const float strength)
    : texture(texture), box(RenderBox(position, extent)), strength(strength), cube_camera(position) {
}
EnvironmentLight::EnvironmentLight(const EnvironmentLight &light) : texture(light.texture), box(light.box), strength(light.strength), cube_camera(light.cube_camera) {}
EnvironmentLight &EnvironmentLight::operator=(const EnvironmentLight &other) {
  texture = other.texture;
  box = other.box;
  strength = other.strength;
  cube_camera = other.cube_camera;
  return *this;
}

}