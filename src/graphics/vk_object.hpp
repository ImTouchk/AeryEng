#pragma once

#include "utils/types.hpp"
#include "math/vec2.hpp"
#include "math/vec3.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <vector>
#include <array>

namespace Aery {
    struct VkShader;
    struct VkVertex {
        vec3 position;
        vec3 color;

        static std::array<vk::VertexInputAttributeDescription, 2> GetAttributeDescription() {
            std::array<vk::VertexInputAttributeDescription, 2> Description = {};
            vk::VertexInputAttributeDescription Attribute = {
                .location = 0,
                .binding = 0,
                .format = vk::Format::eR32G32B32Sfloat,
                .offset = offsetof(VkVertex, position)
            };
            Description[0] = Attribute;
            Description[1] = Attribute;
            Description[1].offset = offsetof(VkVertex, color);
            Description[1].location = 1;
            return Description;
        }

        static vk::VertexInputBindingDescription GetBindingDescription() {
            vk::VertexInputBindingDescription Description = {
                .binding = 0,
                .stride = sizeof(VkVertex),
                .inputRate = vk::VertexInputRate::eVertex
            };
            return Description;
        }
    };

    struct VkObjectCreateInfo {
        std::vector<VkVertex> vertices;
        std::vector<mut_u16> indices;
        VkShader* shader;
    };

    class VkObject {
    public:
        struct {
            std::vector<VkVertex> list;
            vk::Buffer buffer;
            vk::DeviceMemory memory;
            VmaAllocation allocation;
        } vertex;
        std::vector<mut_u16> indices;
        VkShader* shader;
        mut_u32 id;

        bool operator==(const VkObject& Other) {
            return id == Other.id;
        }
    };
}