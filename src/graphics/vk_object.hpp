#pragma once

#include "utils/types.hpp"
#include "math/vec2.hpp"
#include <vulkan/vulkan.hpp>
#include <vector>

namespace Aery {
    struct VkShader;
    class VkObject {
    public:
        std::vector<mut_u16> indices;
        std::vector<vec2> vertices;
        VkShader* shader;
        mut_u32 id;
    };
}