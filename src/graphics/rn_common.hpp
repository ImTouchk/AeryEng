#pragma once

#include "utils/types.hpp"
#include "math/vec3.hpp"
#include "math/mat4.hpp"
#include <string_view>
#include <variant>
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
            mut_u16 reference_count = 0;
            mut_u16 id;

            bool operator==(const Shader& Other) {
                return id = Other.id;
            }
        };

        class Object {
        public:
            std::vector<Vertex> vertices;
            std::vector<mut_u16> indices;
            PushConstant push_constant;
            PShader shader = 0;
            mut_u16 id = 0;

            void* push_data = nullptr;

            bool operator==(const Object& Other) { 
                return id == Other.id;
            }
        };

        struct ShaderCreateInfo {
            const char* vertex_path;
            const char* fragment_path;
        };

        struct ObjectCreateInfo {
            std::vector<Vertex> vertices;
            std::vector<mut_u16> indices;
            std::variant<ShaderCreateInfo, PShader> shader;
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
