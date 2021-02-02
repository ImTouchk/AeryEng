#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vk_object.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <mutex>

using namespace std;

static Aery::mut_u32 Index = 0;
static mutex ListMutex = {};
static mutex PoolMutex = {};

namespace Aery {
    bool VkRenderer::createObject(VkObject* Output, VkShader& Input) {
        VkObject NewObject = {};
        ListMutex.lock();

        ListMutex.unlock();
        return true;
    }
}