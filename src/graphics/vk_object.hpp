#pragma once

#include "utils/types.hpp"
#include <vulkan/vulkan.hpp>
#include <vector>

namespace Aery {
    struct VkShader;
    class VkObject {
    public:
        std::vector<vk::CommandBuffer> cmdBuffers;
        VkShader* shader;
        mut_u32 id;
    };
}