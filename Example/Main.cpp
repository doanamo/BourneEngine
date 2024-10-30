#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Platform/Timer.hpp"
#include "Engine/Graphics/System.hpp"
#include "Engine/Graphics/Stats.hpp"

int main()
{
    Engine::Setup();

    Platform::Window window;
    constexpr char windowTitle[] = "Bourne Engine Example";
    if(!window.Open(windowTitle, 1024, 576))
    {
        LOG_FATAL("Failed to setup platform window");
        return -1;
    }

    Graphics::System graphics;
    if(!graphics.Setup(&window))
    {
        LOG_FATAL("Failed to setup graphics system");
        return -1;
    }

    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
    ComPtr<ID3D11InputLayout> vertexLayout;
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
    #ifdef ENABLE_GRAPHICS_DEBUG
        compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    #endif

        ASSERT_EVALUATE(SUCCEEDED(D3DCompile(shaderCode, ArraySize(shaderCode), nullptr,
            nullptr, nullptr, "VSMain", "vs_4_0", compileFlags, 0, &vertexShaderBlob, nullptr)));

        ASSERT_EVALUATE(SUCCEEDED(graphics.GetDevice()->CreateVertexShader(vertexShaderBlob->GetBufferPointer(),
            vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader)));

        ASSERT_EVALUATE(SUCCEEDED(D3DCompile(shaderCode, ArraySize(shaderCode), nullptr,
            nullptr, nullptr, "PSMain", "ps_4_0", compileFlags, 0, &pixelShaderBlob, nullptr)));

        ASSERT_EVALUATE(SUCCEEDED(graphics.GetDevice()->CreatePixelShader(pixelShaderBlob->GetBufferPointer(),
            pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader)));
    
        D3D11_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        ASSERT_EVALUATE(SUCCEEDED(graphics.GetDevice()->CreateInputLayout(inputElementDescs, ArraySize(inputElementDescs),
            vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &vertexLayout)));
    }

    struct Vertex
    {
        f32 position[3];
        f32 color[4];
    };

    ComPtr<ID3D11Buffer> vertexBuffer;
    {
        Vertex vertices[] =
        {
            {{  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
            {{  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
            {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}
        };

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(Vertex) * ArraySize(vertices);
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA resourceData = {};
        resourceData.pSysMem = vertices;

        ASSERT_EVALUATE(SUCCEEDED(graphics.GetDevice()->CreateBuffer(&bufferDesc, &resourceData, &vertexBuffer)));
    }

    LOG_INFO("Starting main loop...");

    Platform::Timer timer;
    while(true)
    {
        float deltaTime = timer.Tick().GetSeconds();
        window.ProcessEvents();
        if(!window.IsOpen())
            break;

        graphics.BeginFrame();
        {
            graphics.GetDeviceContext()->VSSetShader(vertexShader.Get(), nullptr, 0);
            graphics.GetDeviceContext()->PSSetShader(pixelShader.Get(), nullptr, 0);
            graphics.GetDeviceContext()->IASetInputLayout(vertexLayout.Get());

            u32 stride = sizeof(Vertex);
            u32 offset = 0;
            graphics.GetDeviceContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
            graphics.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            graphics.GetDeviceContext()->Draw(3, 0);
        }
        graphics.EndFrame();

        const Graphics::Stats& graphicsStats = graphics.GetStats();
        if(graphicsStats.HasUpdated())
        {
            auto title = InlineString<64>::Format("%s - %.2f FPS (%.2f ms)",
                windowTitle, graphicsStats.GetFramesPerSecond(),
                graphicsStats.GetFrameTimeAverage() * 1000.0f);
            window.SetTitle(title.GetData());
        }
    }

    LOG_INFO("Exiting application...");
    return 0;
}
