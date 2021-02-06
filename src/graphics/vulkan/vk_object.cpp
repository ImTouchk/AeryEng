#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <mutex>

using namespace std;

static Aery::mut_u32 Index = 1;
static mutex ListMutex = {};

namespace Aery { namespace Graphics {
    bool VkRenderer::createObject(ObjectCreateInfo& Input, PObject* Output) {
        auto CreateIndexBuffer = [&](VkObject& Object) {
            vk::BufferCreateInfo TempBufferInfo = {
                .size = sizeof(Object.indices[0]) * Object.indices.size(),
                .usage = vk::BufferUsageFlagBits::eIndexBuffer
            };

            VmaAllocationCreateInfo AllocateInfo = {
                .usage = VMA_MEMORY_USAGE_CPU_ONLY
            };

            VkBuffer TempBuffer;
            VmaAllocation TempAllocation;
            VkBufferCreateInfo BufferCreateInfo = static_cast<VkBufferCreateInfo>(TempBufferInfo);
            vmaCreateBuffer(m_Allocator, &BufferCreateInfo, &AllocateInfo, &TempBuffer, &TempAllocation, nullptr);

            void* IndexData;
            vmaMapMemory(m_Allocator, TempAllocation, &IndexData);
            memcpy(IndexData, Object.indices.data(), (size_t)TempBufferInfo.size);
            vmaUnmapMemory(m_Allocator, TempAllocation);

            AllocateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

            VkBuffer Buffer;
            VmaAllocation Allocation;
            vmaCreateBuffer(m_Allocator, &BufferCreateInfo, &AllocateInfo, &Buffer, &Allocation, nullptr);

            // Copy from staging buffer to the actual one
            vk::CommandBufferAllocateInfo CmdAllocInfo = {
                .commandPool = m_CommandPool,
                .level = vk::CommandBufferLevel::ePrimary,
                .commandBufferCount = 1,
            };

            vk::CommandBuffer CmdBuffer = m_Device.allocateCommandBuffers(CmdAllocInfo)[0];
            vk::CommandBufferBeginInfo CmdBeginInfo = {
                .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
            };
            CmdBuffer.begin(CmdBeginInfo);

            vk::BufferCopy CopyRegion = {
                .srcOffset = 0,
                .dstOffset = 0,
                .size = TempBufferInfo.size,
            };
            CmdBuffer.copyBuffer(static_cast<vk::Buffer>(TempBuffer), static_cast<vk::Buffer>(Buffer), 1, &CopyRegion);
            CmdBuffer.end();

            vk::SubmitInfo SubmitInfo = {
                .commandBufferCount = 1,
                .pCommandBuffers = &CmdBuffer
            };
            m_GraphicsQ.submit(1, &SubmitInfo, {});
            m_GraphicsQ.waitIdle();
            m_Device.freeCommandBuffers(m_CommandPool, 1, &CmdBuffer);

            Object.index.allocation = Allocation;
            Object.index.buffer = static_cast<vk::Buffer>(Buffer);
            vmaDestroyBuffer(m_Allocator, TempBuffer, TempAllocation);
        };

        auto CreateVertexBuffer = [&](VkObject& Object) {
            //
            vk::BufferCreateInfo TempBufferInfo = {
                .size = sizeof(Object.vertices[0]) * Object.vertices.size(),
                .usage = vk::BufferUsageFlagBits::eVertexBuffer
            };

            // Create staging buffer
            VmaAllocationCreateInfo AllocateInfo = {
                .usage = VMA_MEMORY_USAGE_CPU_ONLY
            };

            VkBuffer TempBuffer;
            VmaAllocation TempAllocation;
            VkBufferCreateInfo BufferCreateInfo = static_cast<VkBufferCreateInfo>(TempBufferInfo);
            vmaCreateBuffer(m_Allocator, &BufferCreateInfo, &AllocateInfo, &TempBuffer, &TempAllocation, nullptr);

            void* VertexData;
            vmaMapMemory(m_Allocator, TempAllocation, &VertexData);
            memcpy(VertexData, Object.vertices.data(), (size_t)TempBufferInfo.size);
            vmaUnmapMemory(m_Allocator, TempAllocation);

            // Create actual buffer
            AllocateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
            
            VkBuffer Buffer;
            VmaAllocation Allocation;
            vmaCreateBuffer(m_Allocator, &BufferCreateInfo, &AllocateInfo, &Buffer, &Allocation, nullptr);
            
            // Copy from staging buffer to the actual one
            vk::CommandBufferAllocateInfo CmdAllocInfo = {
                .commandPool = m_CommandPool,
                .level = vk::CommandBufferLevel::ePrimary,
                .commandBufferCount = 1,
            };

            vk::CommandBuffer CmdBuffer = m_Device.allocateCommandBuffers(CmdAllocInfo)[0];
            vk::CommandBufferBeginInfo CmdBeginInfo = {
                .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
            };
            CmdBuffer.begin(CmdBeginInfo);

            vk::BufferCopy CopyRegion = {
                .srcOffset = 0,
                .dstOffset = 0,
                .size = TempBufferInfo.size,
            };
            CmdBuffer.copyBuffer(static_cast<vk::Buffer>(TempBuffer), static_cast<vk::Buffer>(Buffer), 1, &CopyRegion);
            CmdBuffer.end();

            vk::SubmitInfo SubmitInfo = {
                .commandBufferCount = 1,
                .pCommandBuffers = &CmdBuffer
            };
            m_GraphicsQ.submit(1, &SubmitInfo, {});
            m_GraphicsQ.waitIdle();
            m_Device.freeCommandBuffers(m_CommandPool, 1, &CmdBuffer);

            Object.vertex.allocation = Allocation;
            Object.vertex.buffer = static_cast<vk::Buffer>(Buffer);
            vmaDestroyBuffer(m_Allocator, TempBuffer, TempAllocation);
        };

        mut_u32 ID = 0;
        ListMutex.lock();
            m_Objects.insert(pair<mut_u32, VkObject>(Index, {}));
            m_Objects[Index].id = Index; ID = Index;
            Index++;
        ListMutex.unlock();

        m_Objects[ID].indices = Input.indices;
        m_Objects[ID].vertices = Input.vertices;
        if (Input.shaders.empty()) {
            createDefaultShader(m_Objects[ID].shaders[0]);
        }
        CreateVertexBuffer(m_Objects[ID]);
        CreateIndexBuffer(m_Objects[ID]);

        if (Output != nullptr) {
            *Output = ID;
        }

        Aery::log(fmt::format("<VkRenderer::createObject> ID {} created object {}.", m_ID, ID));
        return true;
    }

    void VkRenderer::destroyObject(PObject Input) {
        assert(m_Objects.contains(Input));
        VkObject& Object = m_Objects[static_cast<mut_u16>(Input)];
        
        vmaDestroyBuffer(m_Allocator, static_cast<VkBuffer>(Object.vertex.buffer), Object.vertex.allocation);
        vmaDestroyBuffer(m_Allocator, static_cast<VkBuffer>(Object.index.buffer), Object.index.allocation);
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
            vmaDestroyBuffer(m_Allocator, static_cast<VkBuffer>(Object.index.buffer), Object.index.allocation);
        }
        m_Objects.clear();

        ListMutex.unlock();
    }
}
}