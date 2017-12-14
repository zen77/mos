#include <mos/render/texture.hpp>
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace mos {
std::atomic_uint Texture::current_id_;
Texture::Texture(const int width,
                 const int height,
                 const Texture::Format &format,
                 const Texture::Wrap &wrap,
                 const bool mipmaps) : id_(current_id_++),
                                        width_(width),
                                        height_(height),
                                        format(format),
                                        wrap(wrap),
                                        mipmaps(mipmaps),
                                        layers_{Data()} {}

Texture::Texture(const std::initializer_list<Texture::Data> &layers,
                 const int width,
                 const int height,
                 const Texture::Format &format,
                 const Texture::Wrap &wrap,
                 const bool mipmaps) : Texture(layers.begin(),
                                                layers.end(),
                                                width,
                                                height,
                                                format,
                                                wrap,
                                                mipmaps) {}

Texture::Texture(const std::initializer_list<std::string> &paths,
                 const Texture::Format &format,
                 const Texture::Wrap &wrap,
                 const bool mipmaps) : id_(current_id_++), wrap(wrap), format(format), mipmaps(mipmaps) {
  for (auto &path : paths) {
    int bpp;
    unsigned char * pixels = stbi_load(path.c_str(), &width_, &height_, &bpp, STBI_rgb_alpha);
    layers_.push_back(Data(pixels, pixels + (width_ * height_ * 4)));
  }
}

int Texture::id() const {
  return id_;
}
int Texture::width() const {
  return width_;
}
int Texture::height() const {
  return height_;
}
int Texture::depth() const {
  return layers_.size();
}
int Texture::size(const int layer) {
  return layers_[layer].size();
}
const unsigned char *Texture::data(const int layer) const {
  return layers_[layer].data();
}
}
