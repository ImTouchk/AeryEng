#pragma once

#include "utils/types.hpp"
#include "math/vec3.hpp"
#include "math/mat4.hpp"
#include <string_view>
#include <vector>

namespace Aery {
    namespace Graphics {
        class Window;

        struct Vertex {
            vec3 position;
            vec3 color;
        };

        struct PushConstant {
            mat4 transform;
        };

        using PShader = mut_u16;
        using PObject = mut_u16;

        class Shader {
        public:
            mut_u16 id;

            bool operator==(const Shader& Other) {
                return id = Other.id;
            }
        };

        class Object {
        public:
            std::vector<Vertex> vertices;
            std::vector<mut_u16> indices;
            std::vector<Shader> shaders;
            PushConstant push_constant;
            mut_u16 id;

            bool operator==(const Object& Other) { 
                return id == Other.id;
            }
        };

        struct ObjectCreateInfo {
            std::vector<PShader> shaders;
            std::vector<Vertex> vertices;
            std::vector<mut_u16> indices;
        };

        struct ShaderCreateInfo {
            const char* vertex_path;
            const char* fragment_path;
        };

        enum class PresentMode : mut_u16 {
            eTripleBuffering = 0,
            eImmediate = 1 << 0,
            eVsync = 1 << 1, 
            eAny = 1 << 2
        };
        struct RendererCreateInfo {
            PresentMode present_mode = PresentMode::eAny;
            Window* window = nullptr;
        };
    }
}
