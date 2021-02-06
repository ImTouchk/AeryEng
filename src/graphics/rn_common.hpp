#pragma once

#include "utils/types.hpp"
#include "math/vec3.hpp"
#include "math/mat4.hpp"
#include <string_view>
#include <vector>

namespace Aery {
    class Window;
    namespace Graphics {
        struct Vertex {
            vec3 position;
            vec3 color;
        };

        struct PushConstant {
            mat4 transform;
        };

        using PShader = mut_u16;
        using PObject = mut_u16;

        class Object {
        public:
            std::vector<Vertex> vertices;
            std::vector<mut_u16> indices;
            PushConstant push_constant;
            mut_u16 id;

            bool operator==(const Object& Other) { 
                return id == Other.id;
            }
        };

        class Shader {
        public:
            mut_u16 id;

            bool operator==(const Shader& Other) {
                return id = Other.id;
            }
        };

        struct ObjectCreateInfo {
            const std::vector<PShader> shaders;
            const std::vector<Vertex> vertices;
            const std::vector<mut_u16> indices;
        };

        struct ShaderCreateInfo {
            std::string_view vertex_path;
            std::string_view fragment_path;
        };

        enum class PresentModeBits : mut_u16 {
            eTripleBuffering = 0,
            eImmediate = 1 << 0,
            eVsync = 1 << 1, 
            eAny = 1 << 2
        };
        using PresentMode = PresentModeBits;
        struct RendererCreateInfo {
            const PresentMode present_mode = PresentModeBits::eAny;
            const Window* window = nullptr;
        };
    }
}
