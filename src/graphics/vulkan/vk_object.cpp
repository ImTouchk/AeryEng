#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vk_shader.hpp"
#include "graphics/vk_object.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <mutex>

using namespace std;

static Aery::mut_u32 Index = 1;
static mutex ListMutex = {};

namespace Aery {
    bool VkRenderer::createObject(VkObjectCreateInfo& Input, PVkObject* Output) {
        auto CreateVertexBuffer = [&](VkObject& Object ) {
            vk::BufferCreateInfo BufferInfo = {
                .size = sizeof(Object.vertex.list[0]) * Object.vertex.list.size(),
                .usage = vk::BufferUsageFlagBits::eVertexBuffer
            };

            VmaAllocationCreateInfo AllocateInfo = {
                .usage = VMA_MEMORY_USAGE_CPU_TO_GPU
            };

            VkBuffer Buffer;
            VmaAllocation Allocation;
            VkBufferCreateInfo BufferCreateInfo = static_cast<VkBufferCreateInfo>(BufferInfo);
            vmaCreateBuffer(m_Allocator, &BufferCreateInfo, &AllocateInfo, &Buffer, &Allocation, nullptr);

            void* VertexData;
            vmaMapMemory(m_Allocator, Allocation, &VertexData);
            memcpy(VertexData, Object.vertex.list.data(), BufferInfo.size);
            vmaUnmapMemory(m_Allocator, Allocation);

            Object.vertex.allocation = Allocation;
            Object.vertex.buffer = static_cast<vk::Buffer>(Buffer);
        };

        mut_u32 ID = 0;
        ListMutex.lock();
            m_Objects.insert(pair<mut_u32, VkObject>(Index, {}));
            m_Objects[Index].id = Index; ID = Index;
            Index++;
        ListMutex.unlock();

        m_Objects[ID].indices = Input.indices;
        m_Objects[ID].vertex.list = Input.vertices;
        if (Input.shader == 0) {
            createDefaultShader(&m_Objects[ID].shader);
        }
        CreateVertexBuffer(m_Objects[ID]);

        if (Output != nullptr) {
            *Output = ID;
        }

        Aery::log(fmt::format("<VkRenderer::createObject> ID {} created object {}.", m_ID, ID));
        return true;
    }

    void VkRenderer::destroyObject(PVkObject Input) {
        VkObject& Object = m_Objects[Input];

        vmaDestroyBuffer(m_Allocator, static_cast<VkBuffer>(Object.vertex.buffer), Object.vertex.allocation);
        ListMutex.lock();
            m_Objects.erase((mut_u32)Input);
        ListMutex.unlock();
        Aery::log(fmt::format("<VkRenderer::destroyObject> ID {} destroyed object {}.", m_ID, Input));
    }

    void VkRenderer::DestroyObjects() {
        // (Same scenario as for shaders) I am not calling the destroyObject method
        // as it removes the element from the map, resulting in weird out-of-bounds
        // access bugs.
        ListMutex.lock();

        for (auto& Object_ : m_Objects) {
            VkObject& Object = Object_.second;
            vmaDestroyBuffer(m_Allocator, static_cast<VkBuffer>(Object.vertex.buffer), Object.vertex.allocation);
        }
        m_Objects.clear();

        ListMutex.unlock();
    }
}