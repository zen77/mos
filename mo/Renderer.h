/* 
 * File:   Renderer.h
 * Author: morgan
 *
 * Created on February 15, 2014, 2:37 PM
 */

#ifndef OGLI_RENDERER_H
#define	OGLI_RENDERER_H

#include <ogli/ogli.h>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <tuple>

#include "Vertex.h"
#include "Texture2D.h"
#include "Model.h"


namespace mo {

    struct ProgramData {
        ogli::Program program;
        ogli::Uniform mvp;
        ogli::Uniform mv;
        ogli::Uniform texture;
        ogli::Uniform opacity;
        ogli::Uniform light_position;
    };

    class Renderer {
    public:

        typedef std::pair<unsigned int, ogli::ArrayBuffer> ArrayPair;
        typedef std::pair<unsigned int, ogli::ElementArrayBuffer> ElementPair;
        typedef std::pair<unsigned int, ogli::TextureBuffer> TexturePair;
        typedef std::pair<std::string, ProgramData> ProgramPair;

        Renderer();
        void addProgram(const std::string name);
        void addProgram(const std::string path, const std::string vertex_shader_source, const std::string fragment_shader_source);
        void render(const Model & model, const glm::mat4 transform, const glm::mat4 view, const glm::mat4 projection, const float opacity = 1.0f, const std::string program_name = "standard", const glm::vec3 = glm::vec3(0.0f, 0.0f, 0.0f));
        void clear(const glm::vec3 color);

        virtual ~Renderer();
    private:
        ogli::Attribute position_attribute_3P3N2UV_;
        ogli::Attribute normal_attribute_3P3N2UV_;
        ogli::Attribute uv_attribute_3P3N2UV_;

        std::map<std::string, ProgramData> programs_;
        std::map<unsigned int, ogli::TextureBuffer> textures_;
        std::map<unsigned int, ogli::ArrayBuffer> array_buffers_;
        std::map<unsigned int, ogli::ElementArrayBuffer> element_array_buffers_;

    };
}
#endif	/* OGLI_RENDERER_H */

