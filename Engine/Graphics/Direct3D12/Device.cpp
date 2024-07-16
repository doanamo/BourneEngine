#include "Shared.hpp"
#include "Graphics/Device.hpp"
#include "Platform/Window.hpp"

Graphics::Device::~Device()
{
    LOG("Destroying D3D12 device");

    WaitForGPU();
    CloseHandle(m_frameFenceEvent);

#ifdef CONFIG_DEBUG
    class LeakReporter
    {
    public:
        ~LeakReporter()
        {
            ComPtr<IDXGIDebug1> dxgiDebug;
            if(SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
            {
                dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(
                    DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
            }
        }
    };

    static LeakReporter leakReporter;
#endif
}

bool Graphics::Device::Setup(const Platform::Window& window)
{
    if(!CreateDevice())
        return false;

    if(!CreateCommandQueue())
        return false;

    if(!CreateSwapChain(window))
        return false;

    if(!CreateFrameSynchronization())
        return false;

    if(!CreateCommandList())
        return false;

    WaitForGPU();
    return true;
}

void Graphics::Device::BeginFrame()
{
    ASSERT_EVALUATE(SUCCEEDED(m_commandAllocator[m_frameIndex]->Reset()));
    ASSERT_EVALUATE(SUCCEEDED(m_commandList->Reset(m_commandAllocator[m_frameIndex].Get(), nullptr)));

    D3D12_RESOURCE_BARRIER rtvBarrier = {};
    rtvBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    rtvBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    rtvBarrier.Transition.pResource = m_swapChainViews[m_frameIndex].Get();
    rtvBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    rtvBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    rtvBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    m_commandList->ResourceBarrier(1, &rtvBarrier);

    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptorHandle = m_swapChainViewHeap->GetCPUDescriptorHandleForHeapStart();
    rtvDescriptorHandle.ptr += m_frameIndex * m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_commandList->ClearRenderTargetView(rtvDescriptorHandle, clearColor, 0, nullptr);
}

void Graphics::Device::EndFrame()
{
    D3D12_RESOURCE_BARRIER rtvBarrier = {};
    rtvBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    rtvBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    rtvBarrier.Transition.pResource = m_swapChainViews[m_frameIndex].Get();
    rtvBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    rtvBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    rtvBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    m_commandList->ResourceBarrier(1, &rtvBarrier);

    ASSERT_EVALUATE(SUCCEEDED(m_commandList->Close()));

    ID3D12CommandList* commandLists[] = { m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(ArraySize(commandLists), commandLists);

    ASSERT_EVALUATE(SUCCEEDED(m_swapChain->Present(1, 0)));
    AdvanceFrame();
}

bool Graphics::Device::CreateDevice()
{
    UINT dxgiFactoryFlags = 0;

#ifdef CONFIG_DEBUG
    ComPtr<ID3D12Debug1> debugController;
    if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
        debugController->SetEnableGPUBasedValidation(true);
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        LOG_DEBUG("Enabled D3D12 debug layer");
    }
    else
    {
        LOG_WARNING("Failed to enable D3D12 debug layer");
    }
#endif

    if(FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_factory))))
    {
        LOG_ERROR("Failed to create DXGI factory");
        return false;
    }

    LOG("Created DXGI factory");

    if(FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))))
    {
        LOG_ERROR("Failed to create D3D12 device");
        return false;
    }

#ifdef CONFIG_DEBUG
    ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
    if(SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiInfoQueue))))
    {
        ASSERT_EVALUATE(SUCCEEDED(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, true)));
        ASSERT_EVALUATE(SUCCEEDED(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true)));
        ASSERT_EVALUATE(SUCCEEDED(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true)));
    }
#endif

    LOG("Created D3D12 device");
    return true;
}

bool Graphics::Device::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    if(FAILED(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue))))
    {
        LOG_ERROR("Failed to create D3D12 command queue");
        return false;
    }

    LOG("Created D3D12 command queue");
    return true;
}

bool Graphics::Device::CreateSwapChain(const Platform::Window& window)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = SwapChainFrameCount;
    swapChainDesc.Width = window.GetWidth();
    swapChainDesc.Height = window.GetHeight();
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    if(FAILED(m_factory->CreateSwapChainForHwnd(m_commandQueue.Get(), window.GetHandle(), &swapChainDesc, nullptr, nullptr, &swapChain)))
    {
        LOG_ERROR("Failed to create D3D12 swap chain");
        return false;
    }

    ASSERT_EVALUATE(SUCCEEDED(m_factory->MakeWindowAssociation(window.GetHandle(), DXGI_MWA_NO_ALT_ENTER)));

    ASSERT_EVALUATE(SUCCEEDED(swapChain.As(&m_swapChain)));
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

    D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc = {};
    rtvDescriptorHeapDesc.NumDescriptors = SwapChainFrameCount;
    rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if(FAILED(m_device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&m_swapChainViewHeap))))
    {
        LOG_ERROR("Failed to create D3D12 swap chain view heap");
        return false;
    }

    for(u32 i = 0; i < SwapChainFrameCount; ++i)
    {
        if(FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_swapChainViews[i]))))
        {
            LOG_ERROR("Failed to get D3D12 swap chain buffer");
            return false;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptorHandle = m_swapChainViewHeap->GetCPUDescriptorHandleForHeapStart();
        rtvDescriptorHandle.ptr += i * m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        m_device->CreateRenderTargetView(m_swapChainViews[i].Get(), nullptr, rtvDescriptorHandle);

        if(FAILED(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator[i]))))
        {
            LOG_ERROR("Failed to create D3D12 command allocator");
            return false;
        }
    }

    LOG("Created D3D12 swap chain");
    return true;
}

bool Graphics::Device::CreateCommandList()
{
    if(FAILED(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
        m_commandAllocator[m_frameIndex].Get(), nullptr, IID_PPV_ARGS(&m_commandList))))
    {
        LOG_ERROR("Failed to create D3D12 command list");
        return false;
    }
    ASSERT_EVALUATE(SUCCEEDED(m_commandList->Close()));

    LOG("Created D3D12 command list");
    return true;
}

bool Graphics::Device::CreateFrameSynchronization()
{
    if(FAILED(m_device->CreateFence(m_frameFenceValues[m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_frameFence))))
    {
        LOG_ERROR("Failed to create D3D12 frame fence");
        return false;
    }

    m_frameFenceValues[m_frameIndex]++;
    m_frameFenceEvent = CreateEvent(nullptr, false, false, nullptr);
    ASSERT(m_frameFenceEvent != nullptr);

    LOG("Created D3D12 frame synchronization");
    return true;
}

void Graphics::Device::WaitForGPU()
{
    ASSERT_EVALUATE(SUCCEEDED(m_commandQueue->Signal(m_frameFence.Get(), m_frameFenceValues[m_frameIndex])));
    ASSERT_EVALUATE(SUCCEEDED(m_frameFence->SetEventOnCompletion(m_frameFenceValues[m_frameIndex], m_frameFenceEvent)));
    WaitForSingleObjectEx(m_frameFenceEvent, INFINITE, false);
    m_frameFenceValues[m_frameIndex]++;
}

void Graphics::Device::AdvanceFrame()
{
    const u64 currentFenceValue = m_frameFenceValues[m_frameIndex];
    ASSERT_EVALUATE(SUCCEEDED(m_commandQueue->Signal(m_frameFence.Get(), currentFenceValue)));

    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
    if(m_frameFence->GetCompletedValue() < m_frameFenceValues[m_frameIndex])
    {
        ASSERT_EVALUATE(SUCCEEDED(m_frameFence->SetEventOnCompletion(m_frameFenceValues[m_frameIndex], m_frameFenceEvent)));
        WaitForSingleObjectEx(m_frameFenceEvent, INFINITE, false);
    }

    m_frameFenceValues[m_frameIndex] = currentFenceValue + 1;
}