#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Platform/Timer.hpp"
#include "Engine/Graphics/Context.hpp"

int main()
{
    Engine::Setup();

    Platform::Window window;
    constexpr char windowTitle[] = "Example";
    if(!window.Open(windowTitle, 1024, 576))
    {
        LOG_FATAL("Failed to setup platform window");
        return -1;
    }

    Graphics::Context graphics;
    if(!graphics.Setup(window))
    {
        LOG_FATAL("Failed to setup graphics device");
        return -1;
    }

    ID3D12Device10* d3dDevice = graphics.GetDevice();
    ComPtr<ID3D12RootSignature> rootSignature;
    {
        D3D12_ROOT_SIGNATURE_DESC signatureDesc = {};
        signatureDesc.NumParameters = 0;
        signatureDesc.pParameters = nullptr;
        signatureDesc.NumStaticSamplers = 0;
        signatureDesc.pStaticSamplers = nullptr;
        signatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        ComPtr<ID3DBlob> signatureBlob;
        ComPtr<ID3DBlob> errorBlob;
        ASSERT_EVALUATE(SUCCEEDED(D3D12SerializeRootSignature(&signatureDesc,
            D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob)));
        ASSERT_EVALUATE(SUCCEEDED(d3dDevice->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
            signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature))));
    }

    ComPtr<ID3D12PipelineState> pipelineState;
    {
        const char shaderCode[] =
            "struct PSInput" \
            "{" \
            "    float4 position : SV_POSITION;" \
            "    float4 color : COLOR;" \
            "};" \
            "" \
            "PSInput VSMain(float4 position : POSITION, float4 color : COLOR)" \
            "{" \
            "    PSInput result;" \
            "" \
            "    result.position = position;" \
            "    result.color = color;" \
            "" \
            "    return result;" \
            "}" \
            "" \
            "float4 PSMain(PSInput input) : SV_TARGET" \
            "{" \
            "    return input.color;" \
            "}";

        ComPtr<ID3DBlob> vertexShaderBlob;
        ComPtr<ID3DBlob> pixelShaderBlob;

        UINT compileFlags = 0;
#ifdef GRAPHICS_DEBUG
        compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        ASSERT_EVALUATE(SUCCEEDED(D3DCompile(shaderCode, ArraySize(shaderCode), nullptr,
            nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShaderBlob, nullptr)));
        ASSERT_EVALUATE(SUCCEEDED(D3DCompile(shaderCode, ArraySize(shaderCode), nullptr,
            nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShaderBlob, nullptr)));

        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
        inputLayoutDesc.pInputElementDescs = inputElementDescs;
        inputLayoutDesc.NumElements = ArraySize(inputElementDescs);

        D3D12_SHADER_BYTECODE vertesShaderBytecode = {};
        vertesShaderBytecode.pShaderBytecode = vertexShaderBlob->GetBufferPointer();
        vertesShaderBytecode.BytecodeLength = vertexShaderBlob->GetBufferSize();

        D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
        pixelShaderBytecode.pShaderBytecode = pixelShaderBlob->GetBufferPointer();
        pixelShaderBytecode.BytecodeLength = pixelShaderBlob->GetBufferSize();

        D3D12_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
        rasterizerDesc.FrontCounterClockwise = false;
        rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        rasterizerDesc.DepthClipEnable = true;
        rasterizerDesc.MultisampleEnable = false;
        rasterizerDesc.AntialiasedLineEnable = false;
        rasterizerDesc.ForcedSampleCount = 0;
        rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        D3D12_BLEND_DESC blendDesc = {};
        blendDesc.AlphaToCoverageEnable = false;
        blendDesc.IndependentBlendEnable = false;
        for(u32 i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
        {
            blendDesc.RenderTarget[i].BlendEnable = false;
            blendDesc.RenderTarget[i].LogicOpEnable = false;
            blendDesc.RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
            blendDesc.RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
            blendDesc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
            blendDesc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
            blendDesc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
            blendDesc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
            blendDesc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
            blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        }

        D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = false;
        depthStencilDesc.StencilEnable = FALSE;

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = inputLayoutDesc;
        psoDesc.pRootSignature = rootSignature.Get();
        psoDesc.VS = vertesShaderBytecode;
        psoDesc.PS = pixelShaderBytecode;
        psoDesc.RasterizerState = rasterizerDesc;
        psoDesc.BlendState = blendDesc;
        psoDesc.DepthStencilState = depthStencilDesc;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;

        ASSERT_EVALUATE(SUCCEEDED(d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState))));
    }

    ComPtr<ID3D12Resource> vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
    {
        struct Vertex
        {
            float position[3];
            float color[4];
        };

        Vertex vertices[] =
        {
            {{  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
            {{  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
            {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}
        };

        D3D12_HEAP_PROPERTIES heapProperties = {};
        heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
        heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProperties.CreationNodeMask = 1;
        heapProperties.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC resourceDesc = {};
        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resourceDesc.Alignment = 0;
        resourceDesc.Width = ArraySize(vertices) * sizeof(Vertex);
        resourceDesc.Height = 1;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        resourceDesc.SampleDesc.Count = 1;
        resourceDesc.SampleDesc.Quality = 0;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        ASSERT_EVALUATE(SUCCEEDED(d3dDevice->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
            &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer))));

        u8* mappedData = nullptr;
        ASSERT_EVALUATE(SUCCEEDED(vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData))));
        memcpy(mappedData, vertices, sizeof(vertices));
        vertexBuffer->Unmap(0, nullptr);

        vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
        vertexBufferView.StrideInBytes = sizeof(Vertex);
        vertexBufferView.SizeInBytes = sizeof(vertices);
    }

    Platform::Timer timer;

    // #todo: Move framerate counting to separate class
    // #todo: Add min/max frametime tracking/display
    float frameTimeUpdateTimer = 1.0f;
    const u32 frameTimeSamplesMax = 60;
    float frameTimeSamples[frameTimeSamplesMax] = {};
    u32 frameTimeSampleCount = 0;

    while(true)
    {
        timer.Tick();

        frameTimeSamples[frameTimeSampleCount++ % frameTimeSamplesMax] = timer.GetDeltaSeconds();
        frameTimeUpdateTimer -= timer.GetDeltaSeconds();
        if(frameTimeUpdateTimer <= 0.0f)
        {
            float frameTimeAverage = 0.0f;
            const u32 validSampleCount = Min(frameTimeSampleCount, frameTimeSamplesMax);
            for(u32 i = 0; i < validSampleCount; i++)
            {
                if(frameTimeSamples[i] > 0.0f)
                {
                    frameTimeAverage += frameTimeSamples[i];
                }
            }
            frameTimeAverage /= validSampleCount;

            char title[256];
            snprintf(title, sizeof(title), "Example - %.2f FPS (%.2f ms)", 1.0f / frameTimeAverage, frameTimeAverage * 1000.0f);
            window.SetTitle(&title[0]);

            frameTimeUpdateTimer = 1.0f;
            frameTimeSampleCount = 0;
        }

        window.ProcessEvents();
        if(!window.IsOpen())
            break;

        graphics.BeginFrame(window);
        {
            ID3D12GraphicsCommandList* commandList = graphics.GetCommandList();
            commandList->SetGraphicsRootSignature(rootSignature.Get());
            commandList->SetPipelineState(pipelineState.Get());
            commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
            commandList->DrawInstanced(3, 1, 0, 0);
        }
        graphics.EndFrame();
    }

    return 0;
}
