#pragma once

#include "utils/types.hpp"
#include "math/mat4.hpp"
#include <vulkan/vulkan.hpp>

namespace Aery {
    class BaseUBO {
    public:
        virtual vk::DescriptorSetLayoutBinding layout() = 0;
    };
}
