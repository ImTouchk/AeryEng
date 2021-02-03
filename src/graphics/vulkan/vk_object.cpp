#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vk_shader.hpp"
#include "graphics/vk_object.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <mutex>

using namespace std;

static Aery::mut_u32 Index = 0;
static mutex ListMutex = {};

namespace Aery {
    bool VkRenderer::createObject(VkObjectCreateInfo& Input, VkObject** Output) {
        VkObject NewObject = {};
        NewObject.indices = Input.indices;
        NewObject.vertex.list = Input.vertices;
        if (Input.shader == nullptr) {
            createDefaultShader(&NewObject.shader);
        }

        // Vertex buffer creation
        {
            vk::BufferCreateInfo BufferInfo = {
                .size = sizeof(NewObject.vertex.list[0]) * NewObject.vertex.list.size(),
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
            memcpy(VertexData, NewObject.vertex.list.data(), BufferInfo.size);
            vmaUnmapMemory(m_Allocator, Allocation);

            NewObject.vertex.allocation = Allocation;
            NewObject.vertex.buffer = static_cast<vk::Buffer>(Buffer);
        }

        ListMutex.lock();
        NewObject.id = Index; Index++;
        m_Objects.push_back(NewObject);
        if (Output != nullptr) {
            *Output = &m_Objects[m_Objects.size()] - 1;
        }
        ListMutex.unlock();
        Aery::log(fmt::format("<VkRenderer::createObject> ID {} created object {}.", m_ID, NewObject.id));
        return true;
    }

    void VkRenderer::destroyObject(VkObject& Input) {
        vmaDestroyBuffer(m_Allocator, static_cast<VkBuffer>(Input.vertex.buffer), Input.vertex.allocation);
        ListMutex.lock();
        std::vector<VkObject>::iterator Position = std::find(m_Objects.begin(), m_Objects.end(), Input);
        if (Position != m_Objects.end()) {
            m_Objects.erase(Position);
        }
        ListMutex.unlock();
        Aery::log(fmt::format("<VkRenderer::destroyObject> ID {} destroyed object {}.", m_ID, Input.id));
    }
}