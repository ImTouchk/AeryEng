#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vk_shader.hpp"
#include "graphics/vk_object.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <mutex>

using namespace std;

static Aery::mut_u32 Index = 0;
static mutex ListMutex = {};

namespace Aery {
    u32 VkRenderer::FindMemoryType(u32 TypeFilter, vk::MemoryPropertyFlags Properties) {
        vk::PhysicalDeviceMemoryProperties MemoryProperties = {};
        m_PhysicalDevice.getMemoryProperties(&MemoryProperties);

        for (mut_u32 i = 0; i < MemoryProperties.memoryTypeCount; i++) {
            if (TypeFilter & (1 << i) && (MemoryProperties.memoryTypes[i].propertyFlags & Properties) == Properties) {
                return i;
            }
        }
    }

    bool VkRenderer::createObject(VkObjectCreateInfo& Input, VkObject** Output) {
        VkObject NewObject = {};
        NewObject.indices = Input.indices;
        NewObject.vertex.list = Input.vertices;
        if (Input.shader == nullptr) {
            createDefaultShader(&NewObject.shader);
        }

        vk::BufferCreateInfo BufferInfo = {
            .size = sizeof(NewObject.vertex.list[0]) * NewObject.vertex.list.size(),
            .usage = vk::BufferUsageFlagBits::eVertexBuffer,
            .sharingMode = vk::SharingMode::eExclusive
        };

        NewObject.vertex.buffer = m_Device.createBuffer(BufferInfo);

        vk::MemoryRequirements Requirements = {};
        m_Device.getBufferMemoryRequirements(NewObject.vertex.buffer, &Requirements);

        vk::MemoryAllocateInfo AllocateInfo = {
            .allocationSize = Requirements.size,
            .memoryTypeIndex = FindMemoryType(
                Requirements.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eHostVisible | 
                vk::MemoryPropertyFlagBits::eHostCoherent
            )
        };

        NewObject.vertex.memory = m_Device.allocateMemory(AllocateInfo);
        m_Device.bindBufferMemory(NewObject.vertex.buffer, NewObject.vertex.memory, 0);

        void* VertexData = m_Device.mapMemory(NewObject.vertex.memory, 0, BufferInfo.size, {});
        memcpy(VertexData, NewObject.vertex.list.data(), BufferInfo.size);
        m_Device.unmapMemory(NewObject.vertex.memory);

        ListMutex.lock();
        NewObject.id = Index; Index++;
        m_Objects.push_back(NewObject);
        if (Output != nullptr) {
            *Output = &m_Objects[m_Objects.size()] - 1;
        }
        ListMutex.unlock();
        return true;
    }

    void VkRenderer::destroyObject(VkObject& Input) {
        m_Device.destroyBuffer(Input.vertex.buffer);
        m_Device.freeMemory(Input.vertex.memory);
        ListMutex.lock();
        std::vector<VkObject>::iterator Position = std::find(m_Objects.begin(), m_Objects.end(), Input);
        if (Position != m_Objects.end()) {
            m_Objects.erase(Position);
        }
        ListMutex.unlock();
        Aery::log(fmt::format("<VkRenderer::destroyObject> ID {} destroyed object {}.", m_ID, Input.id));
    }
}