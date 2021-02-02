#pragma once

#include "utils/types.hpp"
#include <vulkan/vulkan.hpp>
#include <vector>

namespace Aery {
    struct VkShaderCreateInfo {
        const char* vertex;
        const char* fragment;
    };

    struct VkShader {
        vk::PipelineLayout layout;
        vk::PipelineCache cache;
        vk::Pipeline pipeline;
        mut_u32 id;

        bool operator==(const VkShader& Other) {
            return id == Other.id;
        }
    };
}
