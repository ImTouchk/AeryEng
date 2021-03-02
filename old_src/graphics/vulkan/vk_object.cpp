#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <mutex>

namespace Aery { namespace Graphics {
    bool VkRenderer::createObject(ObjectCreateInfo& Input, PObject* Output) {
        PObject ObjectID = 0;
        VkObject& Object = GetEmptyObjectSlot(&ObjectID);

        Object.id = ObjectID;
        Object.vertices = Input.vertices;
        Object.indices = Input.indices;

        switch (Input.shader.index()) {
        case 0: {
            ShaderCreateInfo ShaderInfo = std::get<ShaderCreateInfo>(Input.shader);
            CreateShader(ShaderInfo, &Object.shader);
            break;
        }
        case 1: {
            Object.shader = std::get<PShader>(Input.shader);
            if (Object.shader == 0) {
                Object.shader = GetDefaultShader();
            }
            break;
        }
        }

        CreateGPUBuffer(vk::BufferUsageFlagBits::eVertexBuffer, Object.vertices.data(), sizeof(Vertex) * Object.vertices.size(),
            m_Device, m_CommandPool, m_Allocator, Object.vertex.allocation, m_GraphicsQ, Object.vertex.buffer);

        CreateGPUBuffer(vk::BufferUsageFlagBits::eIndexBuffer, Object.indices.data(), sizeof(mut_u16)* Object.indices.size(),
            m_Device, m_CommandPool, m_Allocator, Object.index.allocation, m_GraphicsQ, Object.index.buffer);

        if (Output != nullptr) {
            *Output = ObjectID;
        }

        Aery::log(debug_format("<VkRenderer::createObject> ID {} created object {}.", m_ID, ObjectID));
        return true;
    }

    void VkRenderer::destroyObject(PObject Input) {
        VkObject& Object = m_Objects[Input].second;
        vmaDestroyBuffer(m_Allocator, static_cast<VkBuffer>(Object.vertex.buffer), Object.vertex.allocation);
        vmaDestroyBuffer(m_Allocator, static_cast<VkBuffer>(Object.index.buffer), Object.index.allocation);
        Aery::log(debug_format("<VkRenderer::destroyObject> ID {} destroyed object {}.", m_ID, Input));
    }

    void VkRenderer::bindPushConstant(PObject Input, void* PushConstant) {
        VkObject& Object = m_Objects[Input].second;
        Object.push_data = PushConstant;
    }

    void VkRenderer::DestroyObjects() {
        for (auto& Element : m_Objects) {
            VkObject& Object = Element.second;
            vmaDestroyBuffer(m_Allocator, static_cast<VkBuffer>(Object.vertex.buffer), Object.vertex.allocation);
            vmaDestroyBuffer(m_Allocator, static_cast<VkBuffer>(Object.index.buffer), Object.index.allocation);
            Element.first = false;
        }
        m_Objects.resize(8);
    }

    VkObject& VkRenderer::GetEmptyObjectSlot(PObject* Output) {
        for (auto& Element : m_Objects) {
            if (!Element.first) {
                Element.first = {};
                if (Output != nullptr) {
                    *Output = Element.first;
                }
                return Element.second;
            }
        }
        m_Objects.push_back({ true , {} });
        if (Output != nullptr) {
            *Output = m_Objects.size() - 1;
        }
        return m_Objects[m_Objects.size() - 1].second;
    }
}
}