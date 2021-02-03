#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vk_shader.hpp"
#include "graphics/vk_object.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>
#include <fstream>
#include <vector>
#include <mutex>

using namespace std;

static Aery::mut_u32 Index = 1;
static mutex ListMutex = {};

static bool LoadFile(const char* Input, vector<char>& Output, Aery::u32 ID) {
    ifstream Stream = ifstream(Input, ios::ate | ios::binary);
    if (!Stream.is_open()) {
        Aery::log(fmt::format("<VkShader::LoadFile> ID {} failed to load file {}.", ID, Input));
        return false;
    }

    Aery::u32 FileSize = Aery::u32 ( Stream.tellg() );
    if (!Output.empty()) { Output.clear(); }
    Output.resize(FileSize);
    Stream.seekg(0);
    Stream.read(Output.data(), FileSize);
    Stream.close();
    return true;
}

static bool CreateModule(vector<char>& Input, vk::ShaderModule& Output, vk::Device& Device) {
    vk::ShaderModuleCreateInfo ModuleInfo = {
        .codeSize = Input.size(),
        .pCode = reinterpret_cast<Aery::u32*>(Input.data())
    };

    vk::Result Result = Device.createShaderModule(&ModuleInfo, nullptr, &Output);
    if (Result != vk::Result::eSuccess) {
        Aery::error("<VkShader::CreateModule> Failed to create a shader module.");
        return false;
    }
    return true;
}

namespace Aery {
    bool VkRenderer::createDefaultShader(PVkShader* Output) {
        static bool Existent = false;
        static PVkShader DefaultShader = -1;

        if (Existent) {
            if (Output != nullptr) { *Output = DefaultShader; }
            return true;
        }

        VkShaderCreateInfo Default = {
            .vertex = "assets/default_vert.spv",
            .fragment = "assets/default_frag.spv",
        };

        if (!createShader(Default, &DefaultShader)) { return false; }
        if (Output != nullptr) { *Output = DefaultShader; }
        Existent = true;
        return true;
    }

    bool VkRenderer::createShader(VkShaderCreateInfo& Input, PVkShader* Output) {
        auto CreatePipeline = [&](VkShader& Shader) {
            vector<char> VertexCode = {},
                FragmentCode = {};

            if (!LoadFile(Input.vertex, VertexCode, m_ID)) { return false; }
            if (!LoadFile(Input.fragment, FragmentCode, m_ID)) { return false; }

            vk::ShaderModule FragmentModule = {},
                VertexModule = {};

            if (!CreateModule(VertexCode, VertexModule, m_Device)) { return false; }
            if (!CreateModule(FragmentCode, FragmentModule, m_Device)) { return false; }


            vk::PipelineShaderStageCreateInfo VertexCreateInfo = {
                .stage = vk::ShaderStageFlagBits::eVertex,
                .module = VertexModule,
                .pName = "main",
            };

            vk::PipelineShaderStageCreateInfo FragmentCreateInfo = {
                .stage = vk::ShaderStageFlagBits::eFragment,
                .module = FragmentModule,
                .pName = "main",
            };

            vk::PipelineShaderStageCreateInfo ShaderStages[2] = {
                VertexCreateInfo,
                FragmentCreateInfo
            };

            auto BindingDescription = VkVertex::GetBindingDescription();
            auto AttributeDescriptions = VkVertex::GetAttributeDescription();

            vk::PipelineVertexInputStateCreateInfo VertexInputInfo = {
                .vertexBindingDescriptionCount = 1,
                .pVertexBindingDescriptions = &BindingDescription,
                .vertexAttributeDescriptionCount = AttributeDescriptions.size(),
                .pVertexAttributeDescriptions = AttributeDescriptions.data()
            };

            vk::PipelineInputAssemblyStateCreateInfo AssemblyInfo = {
                .topology = vk::PrimitiveTopology::eTriangleList,
                .primitiveRestartEnable = VK_FALSE
            };

            vk::DynamicState PipelineDynamicStates[] = {
                vk::DynamicState::eViewport,
                vk::DynamicState::eScissor
            };

            vk::PipelineDynamicStateCreateInfo DynamicStateCreateInfo = {
                .dynamicStateCount = 2,
                .pDynamicStates = PipelineDynamicStates
            };

            vk::Viewport Viewport = {
                .x = 0, .y = 0,
                .width = (float)m_Swapchain.extent.width,
                .height = (float)m_Swapchain.extent.height,
                .minDepth = 0.0f, .maxDepth = 1.0f
            };

            vk::Rect2D Scissor = {
                .offset = { 0, 0 },
                .extent = m_Swapchain.extent
            };

            vk::PipelineViewportStateCreateInfo ViewportStateInfo = {
                .viewportCount = 1,
                .pViewports = &Viewport,
                .scissorCount = 1,
                .pScissors = &Scissor
            };

            vk::PipelineRasterizationStateCreateInfo RasterStateInfo = {
                .depthClampEnable = VK_FALSE,
                .rasterizerDiscardEnable = VK_FALSE,
                .polygonMode = vk::PolygonMode::eFill,
                .cullMode = vk::CullModeFlagBits::eBack,
                .frontFace = vk::FrontFace::eClockwise,
                .depthBiasEnable = VK_FALSE,
                .depthBiasConstantFactor = 0.0f,
                .depthBiasClamp = 0.0f,
                .depthBiasSlopeFactor = 0.0f,
                .lineWidth = 1.0f,
            };

            vk::PipelineMultisampleStateCreateInfo MultisampleStateInfo = {
                .rasterizationSamples = vk::SampleCountFlagBits::e1,
                .sampleShadingEnable = VK_FALSE,
                .minSampleShading = 1.0f,
                .pSampleMask = nullptr,
                .alphaToCoverageEnable = VK_FALSE,
                .alphaToOneEnable = VK_FALSE,
            };

            using CCFlags = vk::ColorComponentFlagBits;
            vk::PipelineColorBlendAttachmentState BlendAttachmentState = {
                .blendEnable = VK_TRUE,
                .srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
                .dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
                .colorBlendOp = vk::BlendOp::eAdd,
                .srcAlphaBlendFactor = vk::BlendFactor::eOne,
                .dstAlphaBlendFactor = vk::BlendFactor::eZero,
                .alphaBlendOp = vk::BlendOp::eAdd,
                .colorWriteMask = CCFlags::eR | CCFlags::eG | CCFlags::eB | CCFlags::eA,
            };

            vk::PipelineColorBlendStateCreateInfo BlendState = {
                .logicOpEnable = VK_FALSE,
                .logicOp = vk::LogicOp::eCopy,
                .attachmentCount = 1,
                .pAttachments = &BlendAttachmentState,
            };
            BlendState.setBlendConstants({ 0.0f, 0.0f, 0.0f, 0.0f });

            vk::PipelineLayoutCreateInfo LayoutInfo = {
                .setLayoutCount = 0,
                .pSetLayouts = nullptr,
                .pushConstantRangeCount = 0,
                .pPushConstantRanges = nullptr
            };

            vk::Result Result = m_Device.createPipelineLayout(&LayoutInfo, nullptr, &Shader.layout);
            if (Result != vk::Result::eSuccess) {
                Aery::error(fmt::format("<VkRenderer::createShader> ID {} failed to create a pipeline layout.", m_ID));
                return false;
            }

            vk::GraphicsPipelineCreateInfo PipelineInfo = {
                .stageCount = 2,
                .pStages = ShaderStages,
                .pVertexInputState = &VertexInputInfo,
                .pInputAssemblyState = &AssemblyInfo,
                .pViewportState = &ViewportStateInfo,
                .pRasterizationState = &RasterStateInfo,
                .pMultisampleState = &MultisampleStateInfo,
                .pDepthStencilState = nullptr,
                .pColorBlendState = &BlendState,
                .pDynamicState = &DynamicStateCreateInfo,
                .layout = Shader.layout,
                .renderPass = m_RenderPass,
                .subpass = 0,
                .basePipelineHandle = nullptr,
                .basePipelineIndex = -1,
            };
        
            vk::ResultValue<vk::Pipeline> PipelineRes = m_Device.createGraphicsPipeline({}, PipelineInfo);
            if (PipelineRes.result != vk::Result::eSuccess) {
                Aery::warn(fmt::format("<VkRenderer::createShader> ID {} failed to create a pipeline.", m_ID));
                return false;
            }

            Shader.pipeline = PipelineRes.value;
            return true;
        };

        mut_u32 ID = 0;
        ListMutex.lock();
            m_Shaders.insert(std::pair<mut_u32, VkShader>(Index, {}));
            m_Shaders[Index].id = Index; ID = Index;
            Index++;
        ListMutex.unlock();

        if (!CreatePipeline(m_Shaders[ID])) { return false; }

        if (Output != nullptr) { 
            *Output = ID; 
        }

        Aery::log(fmt::format("<VkRenderer::createShader> ID {} created a pipeline {}.", m_ID, ID));
        return true;
    }

    void VkRenderer::destroyShader(PVkShader Input) {
        VkShader& Shader = m_Shaders[Input];

        m_Device.destroyPipeline(Shader.pipeline);
        m_Device.destroyPipelineLayout(Shader.layout);

        ListMutex.lock();
            m_Shaders.erase(Shader.id);
        ListMutex.unlock();

        Aery::log(fmt::format("<VkRenderer::destroyShader> ID {} destroyed pipeline {}.", m_ID, Shader.id));
    }

    VkShader& VkRenderer::getShaderByID(u32 ID) {
        return m_Shaders[ID];
    }

    void VkRenderer::DestroyShaders() {
        // I am not calling 'destroyShader' because it erases the element
        // from the map, which leads to weird out-of-bounds access bugs
        ListMutex.lock();

        for (auto& Shader_ : m_Shaders) {
            VkShader& Shader = Shader_.second;
            m_Device.destroyPipeline(Shader.pipeline);
            m_Device.destroyPipelineLayout(Shader.layout);
        }
        m_Shaders.clear();

        ListMutex.unlock();
    }
}