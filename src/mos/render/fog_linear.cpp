#include <mos/render/fog_linear.hpp>

namespace mos {
FogLinear::FogLinear(const glm::vec3 &color,
                     const float near,
                     const float far,
                     const float linear_factor,
                     const float exponential_factor,
                     const float exponential_attenuation_factor,
                     const float exponential_power)
    : color(color), near(near), far(far),
      linear_factor(linear_factor),
      exponential_factor(exponential_factor),
      exponential_power(exponential_power),
      exponential_attenuation_factor(exponential_attenuation_factor) {}

FogLinear::~FogLinear() {}
}
