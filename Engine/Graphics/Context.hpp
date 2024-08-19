#pragma once

namespace Platform
{
    class Window;
}

namespace Graphics
{
    class Context final
    {
    public:
        Context() = default;
        ~Context();

        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;

        bool Setup(const Platform::Window& window);
        void BeginFrame(const Platform::Window& window);
        void EndFrame();

        u64 GetFrameIndex() const
        {
            return m_frameIndex;
        }

    private:
        u64 m_frameIndex = 0;
        u32 m_swapChainFrameCount = 3;

#ifdef GRAPHICS_DIRECT3D12
    public:
        ID3D12Device10* GetDevice() const
        {
            ASSERT(m_device != nullptr);
            return m_device.Get();
        }

        ID3D12GraphicsCommandList* GetCommandList() const
        {
            ASSERT(m_commandList != nullptr);
            return m_commandList.Get();
        }
    
    private:
        bool CreateDevice();
        bool CreateCommandQueue();
        bool CreateSwapChain(const Platform::Window& window);
        bool CreateFrameSynchronization();

        u64 GetBackBufferIndex() const;

        void WaitForGPU();
        void PresentFrame();

        ComPtr<IDXGIFactory7> m_factory;
        ComPtr<ID3D12Device10> m_device;
        ComPtr<ID3D12CommandQueue> m_commandQueue;
        ComPtr<ID3D12GraphicsCommandList7> m_commandList;
        ComPtr<IDXGISwapChain4> m_swapChain;
        ComPtr<ID3D12DescriptorHeap> m_swapChainViewHeap;
        ComPtr<ID3D12Fence> m_frameFence;
        
        Array<ComPtr<ID3D12CommandAllocator>> m_commandAllocators;
        Array<ComPtr<ID3D12Resource2>> m_swapChainViews;
#endif
    };
}
