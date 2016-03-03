#ifndef MOS_RENDERER_H
#define MOS_RENDERER_H

#include "GL/glew.h"
#include <mos/graphics/texture2d.hpp>
#include <mos/graphics/model.hpp>
#include <mos/graphics/quad.hpp>
#include <mos/graphics/text.hpp>
#include <mos/graphics/particles.hpp>
#include <mos/graphics/light.hpp>
#include <mos/graphics/render_target.hpp>
#include <mos/graphics/camera.hpp>
#include <mos/simulation/box.hpp>
#include <optional.hpp>

#include <unordered_map>
#include <array>

namespace mos {

/**
 * @brief Render geometry.
 * The class that talks to OpenGL, and renders Model objects.
 */
class Renderer {
public:
  using OptTarget = std::optional<Target>;
  /**
   * @brief Renderer constructor.
   * Inits the renderer, in this implementation also creates a
   * valid OpenGL context with GLEW.
   */
  Renderer();
  /**
   * @brief Renderer destructor.
   * Deletes all allocated GPU memory. Textures, Shaders, Buffers.
   */
  virtual ~Renderer();

  /**
   * @brief load a model into renderers own memory.
   * @param model
   */
  void load(const Model &model);

  template <class T>
  /**
   * @brief Load multiple models.
   * @param begin Iterator to first model.
   * @param end Iterator to last model.
   */
  void load(T begin, T end) {
    for (auto it = begin; it != end; it++) {
      load(*it);
    }
  }

  /**
   * @brief unload a model from renderers own memory.
   * @param model
   */
  void unload(const Model &model);

  template <class T>
  /**
   * @brief unload multiple models.
   * @param begin Iterator.
   * @param end Iterator.
   */
  void unload(T begin, T end) {
    for (auto it = begin; it != end; it++) {
      unload(*it);
    }
  }

  /**
   * @brief Load a texture into renderer memory.
   * @param texture The texture.
   */
  void load(const std::shared_ptr<Texture2D> &texture);

  /**
   * @brief unload a texture from renderer memory.
   * @param texture The texture.
   */
  void unload(const std::shared_ptr<Texture2D> &texture);

  // TODO: resolution in rendertarget class or similar.
  void update(const Model &model, const glm::mat4 &view,
              const glm::mat4 &projection, const Light &light = Light(),
              const glm::vec2 &resolution = glm::vec2(0.0f),
              const OptTarget render_target);

  template <class T>
  /**
   * @brief Update render state with multiple models
   * @param begin Iterator to fist model.
   * @param end Iterator to last model
   * @param transform Additional transform matrix.
   * @param view View matrix.
   * @param projection Projection matrix.
   * @param light Dynamic light.
   */
  void update(T begin, T end, const glm::mat4 parent_transform,
              const glm::mat4 view, const glm::mat4 projection,
              const Light &light = Light(),
              const glm::vec2 &resolution = glm::vec2(0.0f)) {
    for (auto it = begin; it != end; ++it) {
      update(*it, parent_transform, view, projection, light, resolution);
    }
  }

  /**
   * @brief Update render state.
   * @param model Model to update.
   * @param camera Camera to render from (view, projection)
   * @param light Dynamic light.
   */
  void update(const Model &model, const Camera &camera,
              const Light &light = Light(),
              const glm::vec2 &resolution = glm::vec2(0.0f));

  template <class T>
  /**
   * @brief Update render state for multiple models.
   * @param begin Iterator to fist model.
   * @param end Iterator to last model.
   * @param camera Camera to render from.
   * @param light Dynamic light.
   */
  void update(T begin, T end, const Camera &camera,
              const Light &light = Light()) {
    for (auto it = begin; it != end; ++it) {
      update(*it, camera, light);
    }
  }

  /**
   * @brief Renders particles.
   * @param particles Particles object.
   * @param view View matrix.
   * @param projection Projection matrix.
   */
  void update(Particles &particles, const glm::mat4 view,
              const glm::mat4 projection);

  void update(const Box &box, const glm::mat4 &view,
              const glm::mat4 &projection);
  void update(const Box &box, const Camera &camera);

  /**
   * @brief Clears the screen and the depth buffer.
   * @param color
   */
  void clear(const glm::vec3 color);

  /**
   * @brief Clear all internal buffers.
   */
  void clear_buffers();
  // TODO: Remove all these from public api!
  /**
   * @brief readFBO
   * @todo remove
   */
  GLuint readFBO;
  /**
   * @brief drawFBO
   * @todo remove
   */
  GLuint drawFBO;
  /**
   * @brief readFBO2
   * @todo remove
   */
  GLuint readFBO2;
  /**
   * @brief drawFBO2
   * @todo remove
   */
  GLuint drawFBO2;

  /**
   * @brief render_target_reset
   * @param width
   * @param height
   */

  /*
   * For multisampled render target
  void render_target_reset(unsigned int width, unsigned int height) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, readFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFBO);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  */

  /**
   * @brief render_target
   * @param target
   */
  void render_target(Target target) {
    if (frame_buffers_.find(target.id()) == frame_buffers_.end()) {

      GLuint frame_buffer_id;
      glGenFramebuffers(1, &frame_buffer_id);
      glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);

      GLuint texture_id;
      glGenTextures(1, &texture_id);
      glBindTexture(GL_TEXTURE_2D, texture_id);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, target.texture->width(),
                   target.texture->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_2D, texture_id, 0);

      GLuint depthrenderbuffer_id;
      glGenRenderbuffers(1, &depthrenderbuffer_id);
      glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer_id);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                            target.texture->width(),
                            target.texture->height());
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                GL_RENDERBUFFER, depthrenderbuffer_id);
      glBindRenderbuffer(GL_RENDERBUFFER, 0);

      // Always check that our framebuffer is ok
      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer incomplete.");
      }
      // textures_.insert(TexturePair(target.texture->id(),
      // ogli::TextureBuffer{renderedTexture}));
      textures_.insert({target.texture->id(), texture_id});
      frame_buffers_.insert({target.id(), frame_buffer_id});

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    auto fb = frame_buffers_[target.id()];
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    glViewport(0, 0, target.texture->width(), target.texture->height());
  }

  /**
   * @brief Set lightmap use.
   */
  void lightmaps(const bool lightmaps);

  /**
   * @brief Check if lightmaps is enabled.
   * @return True if lightmaps are enabled.
   */
  bool lightmaps() const;

private:
  /**
   * @brief The ParticleProgramData struct, uniforms for the particle shader
   * program.
   */
  struct ParticleProgramData {
    GLuint program;
    GLint mvp;
    GLint mv;
  };

  /**
   * @brief The BoxProgramData struct, uniforms for the bounding box shader
   * program.
   */
  struct BoxProgramData {
    GLuint program;
    GLint mvp;
    GLint mv;
  };

  /**
   * @brief The VertexProgramData struct, uniforms for the standard shader.
   */
  struct VertexProgramData {
    GLuint program;
    GLint mvp;
    GLint mv;
    GLint normal_matrix;
    GLint texture;
    GLint texture2;
    GLint lightmap;
    GLint normalmap;
    GLint material_ambient_color;
    GLint material_diffuse_color;
    GLint material_specular_color;
    GLint material_specular_exponent;
    GLint opacity;
    GLint light_position;
    GLint light_diffuse_color;
    GLint light_specular_color;
    GLint has_texture;
    GLint has_texture2;
    GLint has_lightmap;
    GLint has_normalmap;
    GLint has_material;
    GLint receives_light;
    GLint resolution;
  };

  using VertexProgramPair = std::pair<Model::Shader, VertexProgramData>;
  using ParticleProgramPair = std::pair<std::string, ParticleProgramData>;
  using BoxProgramPair = std::pair<std::string, BoxProgramData>;

  bool lightmaps_;

  /**
   * @brief Updates render state of model.
   * @param model
   * @param transform Additional transform matrix.
   * @param view View matrix of the camera
   * @param projection Projection matrix of the camera
   * @param program_name Either "text" or "standard"
   * @param light One dynamic light to use.
   */
  void update(const Model &model, const glm::mat4 transform,
              const glm::mat4 view, const glm::mat4 projection,
              const Light &light = Light(),
              const glm::vec2 &resolution = glm::vec2(0.0f));

  void add_vertex_program(const Model::Shader shader,
                          const std::string vertex_shader_source,
                          const std::string fragment_shader_source);

  void add_particle_program(const std::string name, const std::string vs_source,
                            const std::string fs_source);

  unsigned int create_shader(const std::string source, const unsigned int type);
  bool check_shader(const unsigned int shader);
  bool check_program(const unsigned int program);
  unsigned int create_texture(std::shared_ptr<mos::Texture2D> texture);
  unsigned int
  create_texture_and_pbo(const std::shared_ptr<mos::Texture2D> &texture);
  void add_box_program(const std::string &name, const std::string &vs_source,
                       const std::string &fs_source);

  std::map<Model::Shader, VertexProgramData> vertex_programs_;
  std::unordered_map<std::string, ParticleProgramData> particle_programs_;
  std::unordered_map<std::string, BoxProgramData> box_programs_;

  std::unordered_map<unsigned int, GLuint> frame_buffers_;
  std::unordered_map<unsigned int, GLuint> pixel_buffers_;
  std::unordered_map<unsigned int, GLuint> textures_;
  std::unordered_map<unsigned int, GLuint> array_buffers_;
  std::unordered_map<unsigned int, GLuint> element_array_buffers_;
  std::unordered_map<unsigned int, GLuint> vertex_arrays_;
  unsigned int box_vbo;
  unsigned int box_ebo;
  unsigned int box_va;
};
}
#endif /* MOS_RENDERER_H */
