#ifndef LUNAR_VKCOMMON_H
#define LUNAR_VKCOMMON_H

#include <vulkan/vulkan.h>

namespace Lunar::vk {
    VkInstance& getInstance();
    void destroyInstance();
}

#endif